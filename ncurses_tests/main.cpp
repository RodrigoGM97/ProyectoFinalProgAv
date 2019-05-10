//To compile add -lncurses

#include <ncurses.h>
#include <string>

using namespace std;

int main()
{
    //Initializing stdscr
    initscr();
    cbreak();
    noecho();

    //Creating a window
    int height, width, start_y, start_x;
    height = 10;
    width = 30;
    start_y = start_x = 10;

    WINDOW * win = newwin(height, width, start_y, start_x);
    WINDOW * optionMenu = newwin(height, width, start_y+height+1, start_x);

    //Creating a box inside the window
    box(win, 0,0); //integers specify the character that surrounds the border
    box(optionMenu, 0, 0);
    refresh();
    wrefresh(win);
    wrefresh(optionMenu);

    mvwprintw(win, 1, 1, "Cursor, Window dimensions");//integers specify offset


    //getting window size
    int y, x, yBeg, xBeg, yMax, xMax;

    getyx(win, y, x); //gets the current position of the cursor
    getbegyx(win, yBeg, xBeg); // gets the top left corner of the window
    getmaxyx(win, yMax, xMax); // gets the bottom right corner

    mvwprintw(win, 2, 1, "%d %d %d %d %d %d", y, x, yBeg, xBeg, yMax, xMax);

    wrefresh(win);//refreshes window to apply changes

    //Get user input
    keypad(win, true);//activates the use of arrows and function keys as input
    int c = wgetch(win);

    if(c == KEY_UP)
    {
        mvwprintw(win, 3, 1, "You pressed up");
        wrefresh(win);
    }

    //Option menu


    string choices[3] = {"walk", "jog", "run"};
    int choice;
    int highlight = 0;

    while(1)
    {
        //Printing menu
        for(int i=0; i<3; i++)
        {
            if(i == highlight)
                wattron(win, A_REVERSE);//assign an attribute
                //.c_str() converts strings to chars for printing
            mvwprintw(win, i+4, 1, choices[i].c_str());
            wattroff(win, A_REVERSE);
            
        }

        choice = wgetch(win);

        switch(choice)
        {
            case KEY_UP:
                highlight--;
                if(highlight == -1)
                {
                    highlight = 0;
                }
                break;
            case KEY_DOWN:
                highlight++;
                if(highlight == 3)
                {
                    highlight = 2;
                }
                break;
            default:
                break;
        }

        if(choice == 10)//user pressed enter
        {
            printw("Your choice was %s", choices[highlight].c_str());    
            break;
        }
    }

    printw("Your choice was %s", choices[highlight].c_str());    

    //Closes the ncurses library, restores the memory
    endwin();

    return 0;
}