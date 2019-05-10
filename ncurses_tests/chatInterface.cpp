#include "chatInterface.h"

  
chatInterface::chatInterface()
{
    drawScreen();
}

bool chatInterface::listenToOptions()
{
    int option;
    bool exit = false;
    keypad(stdscr, true);

    while(1)
    {
        option = getch();

        switch (option)
        {
            case KEY_F(1):
                getMessage(curMessage);
                drawScreen();
                break;
            case KEY_F(2):
                exit = true;
                endwin();
                break;
        }

        if(exit)
            break;
    }

    endwin();
    return exit;
}

void chatInterface::getMessage(char * inputStr)
{
    //Move cursor to start position
    move(cursorStartY, cursorStartX);

    echo();

    getstr(inputStr);
    clear();
}

void chatInterface::drawScreen()
{
    //Get screen size
    initscr();
    noecho();
    getmaxyx(stdscr, yMax, xMax);

    cursorStartY = yMax-7;
    cursorStartX = 8;

    //Create windows
    messagesWin = newwin(yMax-9, xMax-12, 1, 6);
    userInputWin = newwin(6, xMax-12, yMax-8, 6);
    box(messagesWin, 0, 0);
    box(userInputWin, 0, 0);

    //print options on top
    mvprintw(0, 6, "F1=Type message\tF2=Exit");

    //Refreshing on screen
    refresh();
    wrefresh(messagesWin);
    wrefresh(userInputWin);
}