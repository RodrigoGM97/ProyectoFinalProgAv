//
// Created by rodrigo on 4/05/19.
//

#include "chat_client.h"
using namespace std;

int main(int argc, char * argv[])
{
    int connection_fd;
    // Start the server
    connection_fd = connectSocket(argv[1], argv[2]);

    cout << "connected to server" << endl;

    close(connection_fd);

    return 0;
}