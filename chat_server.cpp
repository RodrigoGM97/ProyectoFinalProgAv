//
// Created by rodrigo on 2/05/19.
//

#include "chat_server.h"

void waitForConnections(int server_fd)
{
    struct sockaddr_in client_address;
    socklen_t client_addrlen = sizeof client_address;
    char client_presentation[INET_ADDRSTRLEN];
    int client_fd;
    pid_t new_pid;

    while(1)
    {
        client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_addrlen); //Accept incoming client
        if (client_fd == -1)
        {
            close(server_fd);
            close(client_fd);
            perror("ERROR: listen");
        }

        new_pid = fork(); //Fork to enable multiple clients

        if (new_pid == 0)   // Child process
        {
            // Close the main port
            close(server_fd);
            inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, INET_ADDRSTRLEN);
            printf("Connection from: %s, port %i\n", client_presentation, client_address.sin_port);

            // Start the communication loop
            //communicationLoop(client_fd);

            printf("Connection finalized from: %s, port %i\n", client_presentation, client_address.sin_port);

            // Terminate the child process
            close(client_fd);
            exit(EXIT_SUCCESS);
        }
        else if (new_pid > 0)   // Parent process
        {
            close(client_fd);
        }
        else
        {
            perror("ERROR: fork");
            exit(EXIT_FAILURE);
        }
    }

    // Close the server port
    close(server_fd);
}