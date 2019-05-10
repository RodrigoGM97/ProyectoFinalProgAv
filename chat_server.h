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
#include "encryption.h"

#define MAX_QUEUE 5
#define BUFFER_SIZE 9999
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
    int message_len;
} message_t;

void waitForConnections(int server_fd);

void* attentionThread(void* arg);

void write_store_message(message_t msg, string filename);

pair<message_t, int> read_stored_message(char connected_client[]);

void delete_msg_from_file(message_t msg_to_delete);


#endif //PROYECTOFINALPROGAV_CHAT_H
