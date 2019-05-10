//To compile add -lncurses

#include <ncurses.h>
#include <string>

using namespace std;

int main()
{
    //Initializing stdscr
    initscr();

    //Get screen size
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW * win = newwin(6, xMax-12, yMax-8, 5);
    box(win, 0,0); //integers specify the character that surrounds the border
    refresh();
    wrefresh(win);

    //Get user input
    keypad(win, true);//activates the use of arrows and function 

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
            {
                wattron(win, A_REVERSE);//assign an attribute
                //.c_str() converts strings to chars for printing
            }
            mvwprintw(win, i+1, 1, choices[i].c_str());
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
            break;
        }
    }

    printw("Your choice was %s", choices[highlight].c_str());    

    //Closes the ncurses library, restores the memory
    endwin();

    return 0;
}