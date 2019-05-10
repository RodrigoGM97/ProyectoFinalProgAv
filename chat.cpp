#include <ncurses.h>
#include <iostream>
#include <pthread.h>
#include "chatInterface.h"
#include "chatUserSelector.h"

using namespace std;

int main()
{
    chatInterface chat;
    bool exitFlag = false;
    char message[100];
    char destination[30];

    while(1)
    {
        exitFlag = chat.listenToOptions(message, destination);

        chat.printOnChat("hola");

        if(exitFlag == true)
        {
            printf("exit: %d\n", exitFlag);
            break;
        }

        getch();
    }

    /*char user[30];

    chatUserSelector selector;
    selector.getUser(user);*/



    return 0;
}