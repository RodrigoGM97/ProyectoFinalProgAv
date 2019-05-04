//
// Created by rodrigo on 4/05/19.
//

#include "chat_client.h"
using namespace std;
int interrupt_exit = 0;

int main(int argc, char * argv[])
{
    int connection_fd;
    int poll_response;
    char buffer[BUFFER_SIZE];
    int num_cliente;
    int timeout = 500;
    cout << "Ingrese su número de cliente" << endl;
    cin >> num_cliente;
    sprintf(buffer, "%d", num_cliente);
    // Start the server
    connection_fd = connectSocket(argv[1], argv[2]);

    cout << "connected to server" << endl;
    sendString(connection_fd, buffer, BUFFER_SIZE);
    sendString(connection_fd, buffer, BUFFER_SIZE);
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
            // Receive the request
        else if (poll_response > 0)
        {
            if (recvString(connection_fd, buffer, BUFFER_SIZE) == 0)
            {
                cout << "Server unavailable" << endl;
                break;
            }
        }
    }
    close(connection_fd);

    return 0;
}