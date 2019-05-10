//Advanced Programming Final Project
//Chat
//
//10-05-19
//
//Rodrigo Garcia
//A01024595
//
//Saul Labra
//A01020725

#ifndef PROYECTOFINALPROGAV_CHAT_CLIENT_H
#define PROYECTOFINALPROGAV_CHAT_CLIENT_H

//Standard libraries
#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <poll.h>
#include <string>
#include <pthread.h>
#include <string.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
// Custom libraries
#include "sockets.h"
#include "fatal_error.h"
#include "encryption.h"

#define BUFFER_SIZE 9999

using namespace std;

//Struct for message
typedef struct message_struct {
    char account_from[BUFFER_SIZE];
    char account_to[BUFFER_SIZE];
    char message[BUFFER_SIZE];
    int message_len;
} message_t;

// Data that will be sent to each structure
typedef struct data_struct {
    // The file descriptor for the socket
    int connection_fd;
    char client_id[BUFFER_SIZE];

} thread_data_t;

//Thread to handle user input
void* client_write(void* arg);

#endif //PROYECTOFINALPROGAV_CHAT_CLIENT_H
