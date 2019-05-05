//
// Created by rodrigo on 4/05/19.
//

#ifndef PROYECTOFINALPROGAV_CHAT_CLIENT_H
#define PROYECTOFINALPROGAV_CHAT_CLIENT_H


#include <iostream>
// Sockets libraries
#include <netdb.h>
#include <arpa/inet.h>
#include <poll.h>
#include <string>

// Custom libraries
#include "sockets.h"
#include "fatal_error.h"
#define BUFFER_SIZE 200
using namespace std;

typedef struct message_struct {
    char account_from[BUFFER_SIZE];
    char account_to[BUFFER_SIZE];
    char message[BUFFER_SIZE];

} message_t;

#endif //PROYECTOFINALPROGAV_CHAT_CLIENT_H
