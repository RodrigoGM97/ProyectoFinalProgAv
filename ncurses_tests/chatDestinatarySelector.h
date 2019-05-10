#include <ncurses.h>

class chatDestinatarySelector
{
    private:
        int yMax, xMax;
        WINDOW * userEmailInput;
    public:
        chatDestinatarySelector();
        void drawScreen();
        void getUser(char * user);
};