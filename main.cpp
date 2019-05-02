#include "chat_server.h"
#include "sockets.h"

#include <iostream>


int main() {
    std::cout << "Hello, World!" << std::endl;
    printLocalIPs();
    int server = initServer("80080000", MAX_QUEUE);
    std::cout << server << std::endl;
    return 0;
}
