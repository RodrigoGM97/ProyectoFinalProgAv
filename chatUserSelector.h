#include <ncurses.h>

class chatUserSelector
{
    private:
        int yMax, xMax;
        WINDOW * userEmailInput;
    public:
        chatUserSelector();
        void drawScreen();
        void getUser(char * user);
};