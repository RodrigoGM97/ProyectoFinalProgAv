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
    int num_cliente;
    int timeout = 500;

    connection_fd = connectSocket(argv[1], argv[2]);
    cout << "Ingrese su número de cliente" << endl;
    cin >> num_cliente;
    sprintf(buffer, "%d", num_cliente);
    cout << "connected to server" << endl;
    sendString(connection_fd, buffer, BUFFER_SIZE);
    message_t msg;
    msg.account_from = num_cliente;

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
            msg.account_to = "1juan";
            msg.message = "2Jala";
            //sprintf(buffer, "%i %s %s", msg.account_from, msg.account_to.c_str(), msg.message.c_str());
            sprintf(buffer, "%s", msg.message.c_str());
            sendString(connection_fd, buffer, BUFFER_SIZE);
            sprintf(buffer, "%s", msg.account_to.c_str());
            sendString(connection_fd, buffer, BUFFER_SIZE);

            //cout << "Message sent" << endl;
        }
        // Receive the request
        else if (poll_response > 0)
        {
            if (recvString(connection_fd, buffer, BUFFER_SIZE) == 0)
            {
                cout << "Server unavailable" << endl;
                break;
            }
            cout << buffer << endl;

        }
    }
    close(connection_fd);

    return 0;
}