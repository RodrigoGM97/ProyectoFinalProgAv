//
// Created by rodrigo on 2/05/19.
//

#include "chat_server.h"


int interrupt_exit = 0;

int main() {
    std::cout << "Hello, World!" << std::endl;
    printLocalIPs();
    int server = initServer(const_cast<char *>("8989"), MAX_QUEUE);
    //std::cout << server << std::endl;
    waitForConnections(server);
    return 0;
}

void waitForConnections(int server_fd)
{
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char client_presentation[INET_ADDRSTRLEN];
    int client_fd;
    pthread_t new_tid;
    thread_data_t * connection_data = NULL;
    int poll_response;
    int timeout = 500;		// Time in milliseconds (0.5 seconds)

    // Get the size of the structure to store client information
    client_address_size = sizeof client_address;

    while (1)
    {
        // Create a structure array to hold the file descriptors to poll
        struct pollfd test_fds[1];
        // Fill in the structure
        test_fds[0].fd = server_fd;
        test_fds[0].events = POLLIN;    // Check for incomming data

        poll_response = poll(test_fds, 1, timeout);
        //Interruption (system call)
        if (poll_response == -1)
        {
            if (errno == EINTR)
            {
                break;
            }
        }
        //Check for interruptions
        if (poll_response == 0)
        {
            if (interrupt_exit)
                break;
        }
        //Continue bank operations
        if (poll_response > 0)
        {
            if (test_fds[0].revents & POLLIN)
            {
                // ACCEPT
                // Wait for a client connection
                client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_size);
                if (client_fd == -1)
                {
                    fatalError("ERROR: accept");
                }

                // Get the data from the client
                inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
                printf("Received incomming connection from %s on port %d\n", client_presentation, client_address.sin_port);

                // CREATE A THREAD
                if (pthread_create(&new_tid, NULL, attentionThread, connection_data) == 0)
                    printf("Thread created\n");
            }
        }
    }
}

void* attentionThread(void* arg)
{
    cout << "In attention thread" << endl;
    return 0;
}
