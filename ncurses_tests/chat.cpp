#include <ncurses.h>
#include <iostream>
#include <pthread.h>
#include "chatInterface.h"
#include "chatDestinatarySelector.h"

using namespace std;

int main()
{
    /*chatInterface chat;
    bool exitFlag = false;

    while(1)
    {
        exitFlag = chat.listenToOptions();

        if(exitFlag == true)
        {
            printf("exit: %d\n", exitFlag);
            break;
        }
    }*/

    char user[30];

    chatDestinatarySelector selector;
    selector.getUser(user);

    return 0;
}