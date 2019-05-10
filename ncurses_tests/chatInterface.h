#include <ncurses.h>

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
        chatInterface();
        bool listenToOptions();
        void getMessage(char * inputStr);
        void drawScreen();
};