#include <ncurses.h>
#include <iostream>

using namespace std;

int main()
{
    //Initializing ncurses
    initscr();

    //Get screen size
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    //Variables
    int cursorStartY = yMax-7;
    int cursorStartX = 8;

    //Creating windows
    WINDOW * messagesWin = newwin(yMax-9, xMax-12, 1, 6);
    WINDOW * userInputWin = newwin(6, xMax-12, yMax-8, 6);
    box(messagesWin, 0, 0);
    box(userInputWin, 0, 0);

    //Move cursor to start position
    move(cursorStartY, cursorStartX);

    //Refreshing on screen
    refresh();
    wrefresh(messagesWin);
    wrefresh(userInputWin);

    //Enable use of arrows
    keypad(stdscr, true);

    int inputChar, curY, curX;
    char str[100];

    getstr(str);

    mvprintw(0,0,"%s", str);

    /*while(1)
    {
        getyx(stdscr, curY, curX);

        inputChar = getch();

        switch (inputChar)
        {
            case KEY_LEFT:
                move(curY, curX-1);
                if(curX <= 6)
                {
                    move(curY, 9);
                }
                break;
            case KEY_RIGHT:
                move(curY, curX+1);
                if(curX >= xMax-6)
                {
                    move(curY, xMax-9);
                }
                break;
            case 10:
                getstr(str);
                break;
        }

        printw("%s", str);

        refresh();
        wrefresh(messagesWin);
        wrefresh(userInputWin);
    }*/     

    getch();

    endwin();

    return 0;
}