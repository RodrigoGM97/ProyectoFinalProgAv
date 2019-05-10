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
#include <pthread.h>
#include <openssl/aes.h>
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

// Data that will be sent to each structure
typedef struct data_struct {
    // The file descriptor for the socket
    int connection_fd;
    char client_id[BUFFER_SIZE];

} thread_data_t;

void* client_write(void* arg);

message_t encrypt_msg(message_t msg);

message_t decrypt_msg(message_t msg);

#endif //PROYECTOFINALPROGAV_CHAT_CLIENT_H
