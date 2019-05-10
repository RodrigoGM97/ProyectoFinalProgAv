/*
    Begining game using ncurses library.
    Following the instructions at:
        https://www.viget.com/articles/game-programming-in-c-with-the-ncurses-library/
    More info about ncurses:
        http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/index.html

    Gilberto Echeverria
    16/10/2018
*/

#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <pthread.h>

#define DELAY 50000

// Structure for a single point in 2D
typedef struct coords_struct {
    int x;
    int y;
} coords_t;

// Structure with the data that will be shared between threads
typedef struct data_struct {
    coords_t * point;
    coords_t * limits;
} data_t;


void initSharedData(data_t ** shared_data);
int bounceBall(data_t * shared_data);
void * getKeystrokes (void * arg);

int main(int argc, char * argv[])
{
    // Prepare the window
    initscr();
    // Do not show keypresses on screen
    noecho();
    // Get the input immediately
    cbreak();
    // Read data from special keys, like the arrows
    keypad(stdscr, TRUE);
    // Hide the cursor
    curs_set(FALSE);
    
    data_t * shared_data = NULL;
    pthread_t tid;

    // Prepare the shared memory
    initSharedData(&shared_data);
    
    pthread_create(&tid, NULL, getKeystrokes, shared_data);
    
    bounceBall(shared_data);
    

    // Return the termial to normal
    endwin();

    return 0;
}

void initSharedData (data_t ** shared_data)
{
    *shared_data = malloc(sizeof (data_t));
    
    // Init the inner structures
    (*shared_data)->point = malloc(sizeof (coords_t));
    (*shared_data)->limits = malloc(sizeof (coords_t));

    // Set the initial point
    (*shared_data)->point->x = 0;
    (*shared_data)->point->y = 0;
}

int bounceBall(data_t * shared_data)
{
    int next_x;
    int direction = 1;

    while (1)
    {
        // Determine the size of the screen
        // Global var `stdscr` is created by the call to `initscr()`
        getmaxyx(stdscr, shared_data->limits->y, shared_data->limits->x);

        // Clear the screen of all previously-printed characters
        clear(); 
        // Print our "ball" at the current xy position
        mvprintw(shared_data->point->y, shared_data->point->x, "o");
        refresh();

        // Shorter delay between movements
        usleep(DELAY);
        // Advance the ball in the current direction
        next_x = shared_data->point->x + direction;

        // Make sure the ball bounces at the edges
        if ( (next_x > shared_data->limits->x) || (next_x < 0) )
        {
            direction *= -1;
        }
        else
        {
            shared_data->point->x = next_x;
        }
    }

    return 0;
}


void * getKeystrokes(void * arg)
{
    data_t * shared_data = (data_t *) arg;
    int ch;
    
    while(1)
    {
        ch = getch();
        
        switch(ch)
        {
            case KEY_UP:
                if (shared_data->point->y > 0)
                    shared_data->point->y--;
                break;
            case KEY_DOWN:
                if (shared_data->point->y < shared_data->limits->y)
                    shared_data->point->y++;
                break;
            case 'q':
                endwin();
                exit(EXIT_SUCCESS);
                break;
        }
    }
}