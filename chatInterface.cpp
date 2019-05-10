#include "chatInterface.h"

  
chatInterface::chatInterface()
{
    drawScreen();
}

bool chatInterface::listenToOptions(char * message, char * destination)
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
                getDestination(destination);
                getMessage(message);
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

void chatInterface::getDestination(char * destination)
{
    wprintw(userInputWin, "Enter destination");
    wrefresh(userInputWin);

    //Move cursor to start position
    move(cursorStartY, cursorStartX);

    echo();

    getstr(destination);
    wclear(userInputWin);
    box(userInputWin, 0, 0);
    wrefresh(userInputWin);
}

void chatInterface::getMessage(char * message)
{
    wprintw(userInputWin, "Enter message");
    wrefresh(userInputWin);

    //Move cursor to start position
    move(cursorStartY, cursorStartX);

    echo();

    getstr(message);
    wclear(userInputWin);
    box(userInputWin, 0, 0);
    wrefresh(userInputWin);
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

    //enable scrolling
    scrollok(messagesWin, true);

    //print options on top
    mvprintw(0, 6, "F1=Type message\tF2=Exit");

    //Refreshing on screen
    refresh();
    wrefresh(messagesWin);
    wrefresh(userInputWin);
}

void chatInterface::printOnChat(char * input)
{
    getyx(messagesWin, curY, curX);
    move(curY, curX+2);

    for(int i=0; i<100; i++)
    {
        wprintw(messagesWin, "%d Example line\n", i);
        wrefresh(messagesWin);
    }
}