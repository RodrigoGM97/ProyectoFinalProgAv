#include "chatDestinatarySelector.h"

chatDestinatarySelector::chatDestinatarySelector()
{
    drawScreen();
}

void chatDestinatarySelector::drawScreen()
{
    //Get screen size
    initscr();
    getmaxyx(stdscr, yMax, xMax);

    //Create windows
    userEmailInput = newwin(3, xMax/3, yMax/2, xMax/3);
    box(userEmailInput, 0, 0);

    //print Indication on top
    mvprintw(yMax/2-1, xMax/3, "Enter username / email");

    //move cursor to input field
    move(yMax/2+1, xMax/3+2);

    //Refreshing on screen
    refresh();
    wrefresh(userEmailInput);
}

void chatDestinatarySelector::getUser(char * user)
{
    getstr(user);
    endwin();
}