#include <ncurses.h>

typedef struct status_s{

}status_t;

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
        bool listenToOptions(char * message, char * destination);
        void getDestination(char * destination);
        void getMessage(char * message);
        void drawScreen();
        void printOnChat(char * input);
};