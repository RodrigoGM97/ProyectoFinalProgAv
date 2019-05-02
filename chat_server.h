//
// Created by rodrigo on 2/05/19.
//

#ifndef PROYECTOFINALPROGAV_CHAT_H
#define PROYECTOFINALPROGAV_CHAT_H

#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MAX_QUEUE 5
#define BUFFER_SIZE 200
using namespace std;

// Data that will be sent to each structure
typedef struct data_struct {
    // The file descriptor for the socket
    int connection_fd;

} thread_data_t;


#endif //PROYECTOFINALPROGAV_CHAT_H
