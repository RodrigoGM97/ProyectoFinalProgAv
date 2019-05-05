//
// Created by rodrigo on 4/05/19.
//

#include "chat_client.h"

int interrupt_exit = 0;

int main(int argc, char * argv[])
{
    int connection_fd;
    int poll_response;
    char buffer[BUFFER_SIZE];
    char num_cliente[BUFFER_SIZE];
    int timeout = 500;

    connection_fd = connectSocket(argv[1], argv[2]);
    cout << "Ingrese su número de cliente" << endl;
    cin >> num_cliente;
    strcpy(buffer, num_cliente);
    cout << buffer << " connected to server" << endl;
    sendString(connection_fd, buffer, BUFFER_SIZE);
    message_t msg;
    message_t income_msg;
    strcpy(msg.account_from, buffer);

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
        if (poll_response==0)
        {
            strcpy(msg.account_to, "Juan");
            strcpy(msg.message, "Hola");
            int send_m = 0;
            cin >> send_m;
            if (send_m == 1)
                sendString(connection_fd, &msg, sizeof(message_t));
            /*sprintf(buffer, "%s", msg.account_to.c_str());
            sendString(connection_fd, buffer, BUFFER_SIZE);*/

            //cout << "Message sent" << endl;
        }
        // Receive the request
        else if (poll_response > 0)
        {
            if (recvString(connection_fd, &income_msg, sizeof(message_t)) == 0)
            {
                cout << "Server unavailable" << endl;
                break;
            }
            cout << "Message from: " << income_msg.account_from << endl;
            cout << "Message: " << income_msg.message << endl;

        }
    }
    close(connection_fd);

    return 0;
}