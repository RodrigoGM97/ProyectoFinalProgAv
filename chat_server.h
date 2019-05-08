//
// Created by rodrigo on 2/05/19.
//

#ifndef PROYECTOFINALPROGAV_CHAT_H
#define PROYECTOFINALPROGAV_CHAT_H

#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <pthread.h>
#include <string>
#include <map>
#include <fstream>
#include <stdio.h>
#include "sockets.h"
#include "fatal_error.h"
#define MAX_QUEUE 5
#define BUFFER_SIZE 200
using namespace std;

// Data that will be sent to each structure
typedef struct data_struct {
    // The file descriptor for the socket
    int connection_fd;
    string client_id;

} thread_data_t;

typedef struct message_struct {
    char account_from[BUFFER_SIZE];
    char account_to[BUFFER_SIZE];
    char message[BUFFER_SIZE];

} message_t;

void waitForConnections(int server_fd);

void* attentionThread(void* arg);

void write_store_message(message_t msg, string filename);

int read_stored_message();

void delete_msg_from_file(message_t msg_to_delete);


#endif //PROYECTOFINALPROGAV_CHAT_H
