#include <ncurses.h>
#include <iostream>
#include <pthread.h>
//#include "chatInterface.h"
//#include "chatUserSelector.h"
#include <pthread.h>
#include <ncurses.h>
#include <unistd.h>

using namespace std;

typedef struct interface_data_s{
    WINDOW * messagesWin;
    WINDOW * userInputWin;
    int yMax, xMax;//screen size
    int cursorStartY, cursorStartX;
    int curY, curX;
    char curMessage[100];
    char curDestination[30];

    pthread_mutex_t * mutex;

    bool exit;
}interface_data_t;

void drawScreen(interface_data_t * screenData)
{
    //Get screen size
    initscr();
    noecho();
    getmaxyx(stdscr, screenData->yMax, screenData->xMax);

    screenData->cursorStartY = screenData->yMax-7;
    screenData->cursorStartX = 8;

    //Create windows
    screenData->messagesWin = newwin(screenData->yMax-9, screenData->xMax-12, 1, 6);
    screenData->userInputWin = newwin(6, screenData->xMax-12, screenData->yMax-8, 6);
    box(screenData->messagesWin, 0, 0);
    box(screenData->userInputWin, 0, 0);

    //enable scrolling
    scrollok(screenData->messagesWin, true);

    //print options on top
    mvprintw(0, 6, "F1=Type message\tF2=Exit");

    //Refreshing on screen
    refresh();
    wrefresh(screenData->messagesWin);
    wrefresh(screenData->userInputWin);
}

void getDestination(char * destination, interface_data_t * screenData)
{
    pthread_mutex_lock(screenData->mutex);
        wprintw(screenData->userInputWin, "Enter destination");
        wrefresh(screenData->userInputWin);

        //Move cursor to start position
        wmove(screenData->userInputWin,screenData->cursorStartY, screenData->cursorStartX);
        //wmove(screenData->userInputWin,0, 0);
        refresh();
        wrefresh(screenData->userInputWin);

        echo();

        getstr(destination);
        wclear(screenData->userInputWin);
        box(screenData->userInputWin, 0, 0);
        wrefresh(screenData->userInputWin);
    pthread_mutex_unlock(screenData->mutex);

}

void getMessage(char * message, interface_data_t * screenData)
{
    pthread_mutex_lock(screenData->mutex);
        wprintw(screenData->userInputWin, "Enter message");
        wrefresh(screenData->userInputWin);

        //Move cursor to start position
        wmove(screenData->userInputWin, screenData->cursorStartY, screenData->cursorStartX);
        //wmove(screenData->userInputWin, 0, 0);
        refresh();
        wrefresh(screenData->userInputWin);

        echo();

        getstr(message);
        wclear(screenData->userInputWin);
        box(screenData->userInputWin, 0, 0);
        wrefresh(screenData->userInputWin);
    pthread_mutex_unlock(screenData->mutex);
}

void * listenToOptions(void * args)
{
    interface_data_t * screenData = (interface_data_t *) args;

    int option;
    keypad(stdscr, true);

    while(1)
    {
        option = getch();

        switch (option)
        {
            case KEY_F(1):
                getDestination(screenData->curDestination, screenData);
                getMessage(screenData->curMessage, screenData);
                break;
            case KEY_F(2):
                screenData->exit = true;
                endwin();
                break;
        }

        if(screenData->exit)
            break;
    }

    endwin();
}

void printOnChat(interface_data_t * screenData)
{
    getyx(screenData->messagesWin, screenData->curY, screenData->curX);
    //move(screenData->curY, screenData->curX+2);

    for(int i=0; i<100; i++)
    {
        usleep(1000000000000000000000);
        wprintw(screenData->messagesWin, "%d Example line\n", i);
        wrefresh(screenData->messagesWin);
    }
}

int main()
{
    interface_data_t * screenData;
    drawScreen(screenData);

    screenData->exit = false;

    pthread_t new_id;
    pthread_create(&new_id, NULL, listenToOptions, screenData);

    while(screenData->exit != true)
    {
        //printOnChat(screenData);
    }

    endwin();
    
    /*chatInterface chat;
    bool exitFlag = false;
    char message[100];
    char destination[30];

    while(1)
    {
        exitFlag = chat.listenToOptions(message, destination);

        pthread_t new_id;
        pthread_create(&new_id, NULL, chat.printOnChat(), NULL);
        chat.printOnChat("hola");

        if(exitFlag == true)
        {
            printf("exit: %d\n", exitFlag);
            break;
        }

        getch();
    }*/

    /*char user[30];

    chatUserSelector selector;
    selector.getUser(user);*/



    return 0;
}