//
// Created by rodrigo on 2/05/19.
//

#include "chat_server.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    printLocalIPs();
    int server = initServer("80080000", MAX_QUEUE);
    std::cout << server << std::endl;
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
    int transactions = 0;

    // Get the size of the structure to store client information
    client_address_size = sizeof client_address;

    cout << "hola" << endl;
}
