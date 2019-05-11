//Advanced Programming Final Project
//Chat
//
//10-05-19
//
//Rodrigo Garcia
//A01024595
//
//Saul Labra
//A01020725

#include "chat_client.h"

void drawScreen(thread_data_t * screenData);
void * client_write(void* arg);
void getDestination(char * destination, thread_data_t * screenData);
void getMessage(char * message, thread_data_t * screenData);
void askUserScreen(char * user);

//Check if program still running
int interrupt_exit = 0;

int main(int argc, char * argv[])
{
    int connection_fd;
    int poll_response;
    char buffer[BUFFER_SIZE];
    //user ID
    char num_cliente[BUFFER_SIZE];
    int timeout = 500;

    connection_fd = connectSocket(argv[1], argv[2]); //Connecting to server

    askUserScreen(num_cliente);

    strcpy(buffer, num_cliente);
    cout << buffer << " connected to server" << endl;
    sendString(connection_fd, buffer, BUFFER_SIZE); //Send user ID to server
    
    message_t income_msg;
    
    //Data that will be stored in thread
    pthread_t new_tid;
    thread_data_t * connection_data = NULL;
    connection_data = new thread_data_t;
    strcpy(connection_data->client_id, buffer);
    connection_data->connection_fd = connection_fd;

    //configuring GUI
    drawScreen(connection_data);
    
    pthread_create(&new_tid, NULL, client_write, connection_data);
    //While program still running
    
    while (!interrupt_exit)
    {
        // Create a structure array to hold the file descriptors to poll
        struct pollfd test_fds[1];
        // Fill in the structure
        test_fds[0].fd = connection_fd;
        test_fds[0].events = POLLIN;    // Check for incomming data
        poll_response = poll(test_fds, 1, timeout);

        if (poll_response == -1)
        {
            if (errno == EINTR)
            {
                break;
            }
        }
        // Receive a message
        else if (poll_response > 0)
        {
            if (recvString(connection_fd, &income_msg, sizeof(message_t)) == 0)
            {
                cout << "Server unavailable" << endl;
                break;
            }
            //Decrypt message
            unsigned char ciphertext[BUFFER_SIZE];
            unsigned char decryptedtext[BUFFER_SIZE];
            int decrypted_len;

            memcpy((char*)ciphertext,income_msg.message,BUFFER_SIZE);
            decrypted_len = decrypt_msg(ciphertext, income_msg.message_len, decryptedtext);
            decryptedtext[decrypted_len] = '\0';

            //printing on GUI
            getyx(connection_data->messagesWin, connection_data->curY, connection_data->curX);

            wprintw(connection_data->messagesWin, "Message from: %s\n", income_msg.account_from);
            wprintw(connection_data->messagesWin, "Message: %s\n", (char*) decryptedtext);

            wrefresh(connection_data->messagesWin);
        }
    }
    close(connection_fd);

    return 0;
}

void drawScreen(thread_data_t * screenData)
{
    //Color
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);

    //Get screen size
    initscr();
    noecho();
    getmaxyx(stdscr, screenData->yMax, screenData->xMax);

    screenData->inputCursorStartY = screenData->yMax-7;
    screenData->inputCursorStartX = 8;

    //Create windows
    screenData->messagesWin = newwin(screenData->yMax-10, screenData->xMax-12, 2, 6);
    screenData->userInputWin = newwin(5, screenData->xMax-12, screenData->yMax-7, 6);

    //Assign color
    wbkgd(screenData->messagesWin, COLOR_PAIR(1));
    wbkgd(screenData->userInputWin, COLOR_PAIR(1));

    //enable scrolling
    scrollok(screenData->messagesWin, true);

    //print options on top
    mvprintw(0, 6, "F1=Type message\tF2=Exit");

    //Refreshing on screen
    refresh();
    wrefresh(screenData->messagesWin);
    wrefresh(screenData->userInputWin);
}

//Thread to manage input messages
void* client_write(void* arg)
{
    //Receive user data
    auto * data = (thread_data_t*)arg;
    char new_msg[BUFFER_SIZE];
    char msg_dest[BUFFER_SIZE];
    message_t msg;
    strcpy(msg.account_from, data->client_id);
    cin.ignore();

    //run

    int option;
    keypad(stdscr, true);

    //While program still running
    while(!interrupt_exit)
    {
        option = getch();

        switch (option)
        {
            case KEY_F(1):
                getDestination(msg_dest, data);
                getMessage(new_msg, data);
                break;
            case KEY_F(2):
                interrupt_exit = true;
                endwin();
                break;
        }

        strcpy(msg.account_to, msg_dest);
        strcpy(msg.message, new_msg);
        //Encrypt message
        int ciphertext_len;
        unsigned char ciphertext[BUFFER_SIZE];

        /* Buffer for the decrypted text */
        unsigned char plaintext[BUFFER_SIZE];

        memcpy((char*)plaintext,msg.message,BUFFER_SIZE);
        ciphertext_len = encrypt_msg(plaintext, strlen((char*)plaintext), ciphertext);
        memcpy(msg.message, (char*)ciphertext, BUFFER_SIZE);
        msg.message_len = ciphertext_len;
        //Send encrypted message
        sendString(data->connection_fd, &msg, sizeof(message_t));
    }

    pthread_exit(NULL);

    endwin();
}

//get destinatary username
void getDestination(char * destination, thread_data_t * screenData)
{
    wprintw(screenData->userInputWin, "Enter destination\n\n");
    wrefresh(screenData->userInputWin);

    //Move cursor to start position
    wrefresh(screenData->userInputWin);

    echo();
    
    wmove(screenData->userInputWin, screenData->inputCursorStartY, screenData->inputCursorStartX);
    refresh();
    wgetstr(screenData->userInputWin, destination);
    wclear(screenData->userInputWin);
    wrefresh(screenData->userInputWin);
}

//get message to send
void getMessage(char * message, thread_data_t * screenData)
{
    wprintw(screenData->userInputWin, "Enter message\n\n");
    wrefresh(screenData->userInputWin);

    //Move cursor to start position
    wmove(screenData->userInputWin, screenData->inputCursorStartY, screenData->inputCursorStartX);
    refresh();
    wrefresh(screenData->userInputWin);

    echo();

    wgetstr(screenData->userInputWin, message);
    wclear(screenData->userInputWin);
    wrefresh(screenData->userInputWin);
}

void askUserScreen(char * user)
{
    WINDOW * userEmailInput;
    int yMax, xMax;
    //Get screen size
    initscr();
    getmaxyx(stdscr, yMax, xMax);

    //Create windows
    userEmailInput = newwin(3, xMax/3, yMax/2, xMax/3);
    box(userEmailInput, 0, 0);

    //print Indication on top
    mvprintw(yMax/2-1, xMax/3, "Enter username / email");

    //move cursor to input field
    move(yMax/2+1, xMax/3+2);

    //Refreshing on screen
    refresh();
    wrefresh(userEmailInput);

    getstr(user);
    endwin();
}