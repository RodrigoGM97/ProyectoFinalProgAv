#include <ncurses.h>
#include <iostream>
#include <pthread.h>

using namespace std;

class chatInterface
{
    private:
        WINDOW * messagesWin;
        WINDOW * userInputWin;
        int yMax, xMax;//screen size
        int cursorStartY, cursorStartX;
        int curY, curX;
        char curMessage[100];
    public:
        chatInterface()
        {
            drawScreen();
        }

        bool listenToOptions()
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

            return exit;
        }

        void getMessage(char * inputStr)
        {
            //Move cursor to start position
            move(cursorStartY, cursorStartX);

            echo();

            getstr(inputStr);
            clear();
        }

        void drawScreen()
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
};



int main()
{
    chatInterface chat;
    bool exitFlag = false;

    while(1)
    {
        exitFlag = chat.listenToOptions();

        if(exitFlag == true)
        {
            printf("exit: %d\n", exitFlag);
            break;
        }
    }   

    endwin();

    return 0;
}