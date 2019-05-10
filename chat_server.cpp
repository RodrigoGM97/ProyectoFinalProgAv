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

#include "chat_server.h"

//Check if program still running
int interrupt_exit = 0;
//Map with connected users and its connectionfd
map<string, int>connected_users;

int main() {
    printLocalIPs();
    //Initiate chat server
    int server = initServer(const_cast<char *>("8989"), MAX_QUEUE);
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
    //Data that will be passed to the thread that will listen to the client
    thread_data_t * connection_data = NULL;
    int poll_response;
    int timeout = 500;  // Time in milliseconds (0.5 seconds)

    // Get the size of the structure to store client information
    client_address_size = sizeof client_address;

    while (1)
    {
        // Create a structure array to hold the file descriptors to poll
        struct pollfd test_fds[1];
        // Fill in the structure
        test_fds[0].fd = server_fd;
        test_fds[0].events = POLLIN;    // Check for incomming data

        poll_response = poll(test_fds, 1, timeout);
        //Interruption (system call)
        if (poll_response == -1)
        {
            if (errno == EINTR)
            {
                break;
            }
        }
        //Check for interruptions
        if (poll_response == 0)
        {
            if (interrupt_exit)
                break;
        }
        //Check for incoming clients
        if (poll_response > 0)
        {
            if (test_fds[0].revents & POLLIN)
            {
                // ACCEPT
                // Wait for a client connection
                client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_size);
                //cout << "Client fd: " << client_fd << endl;
                if (client_fd == -1)
                {
                    fatalError("ERROR: accept");
                }

                // Get the data from the client
                inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
                printf("Received incomming connection from %s on port %d\n", client_presentation, client_address.sin_port);
                //Save the connection data to the thread struct
                connection_data = new thread_data_t;
                connection_data->connection_fd = client_fd;

                // CREATE A THREAD
                if (pthread_create(&new_tid, NULL, attentionThread, connection_data) == 0)
                    printf("Thread created\n");
            }
        }
    }
}

void* attentionThread(void* arg)
{
    int poll_response;
    int timeout = 500;  // Time in milliseconds (0.5 seconds)
    //Retrieve thread data struct
    auto * data = (thread_data_t*)arg;
    char buffer[BUFFER_SIZE];
    //Receive mail from user
    recvString(data->connection_fd, buffer, BUFFER_SIZE);
    data->client_id = buffer;
    cout << data->client_id << endl;
    //Insert the user to the connected user list
    connected_users.insert(pair<string, int>(buffer, data->connection_fd));
    //Check if the user received a message while he was offline
    pair<message_t, int> now_connected_data;
    now_connected_data = read_stored_message(buffer);
    while (now_connected_data.second == 1)
    {
        cout << "User has now logged in, sending message" << endl;
        //Send the message to the user
        int ciphertext_len;
        unsigned char ciphertext[BUFFER_SIZE];

        /* Buffer for the decrypted text */
        unsigned char plaintext[BUFFER_SIZE];

        memcpy((char*)plaintext,now_connected_data.first.message,BUFFER_SIZE);
        ciphertext_len = encrypt_msg(plaintext, strlen((char*)plaintext), ciphertext);
        memcpy(now_connected_data.first.message, (char*)ciphertext, BUFFER_SIZE);
        now_connected_data.first.message_len = ciphertext_len;
        sendString(connected_users.find(now_connected_data.first.account_to)->second, &now_connected_data.first,sizeof(message_t));
        now_connected_data = read_stored_message(buffer);
    }

    //Struct to store the incoming messages
    message_t msg;

    //While server still running
    while (!interrupt_exit)
    {
        // Create a structure array to hold the file descriptors to poll
        struct pollfd test_fds[1];
        // Fill in the structure
        test_fds[0].fd = data->connection_fd;
        test_fds[0].events = POLLIN;    // Check for incomming data
        poll_response = poll(test_fds, 1, timeout);

        if (poll_response == -1)
        {
            if (errno == EINTR)
            {
                break;
            }
        }
        // Receive message from client
        else if (poll_response > 0)
        {
            //Check if client is still online
            if (recvString(data->connection_fd, &msg, sizeof(message_t)) == 0)
            {
                cout << "Client disconnected" << endl;
                //If client no longer online, remove him from the connected users list
                connected_users.erase(data->client_id);
                break;
            }
            //If user is online send him the incoming message
            if (connected_users.find(msg.account_to)->second != 0)
                sendString(connected_users.find(msg.account_to)->second, &msg,sizeof(message_t));
            //If user is not online store his message in the temporal messages
            else if (connected_users.find(msg.account_to)->second == 0)
            {
                //Decrypt message
                unsigned char ciphertext[BUFFER_SIZE];
                unsigned char decryptedtext[BUFFER_SIZE];
                int decrypted_len;

                memcpy((char*)ciphertext,msg.message,BUFFER_SIZE);
                decrypted_len = decrypt_msg(ciphertext, msg.message_len, decryptedtext);
                decryptedtext[decrypted_len] = '\0';
                memcpy(msg.message, (char*)decryptedtext,BUFFER_SIZE);
                write_store_message(msg, "temporal_msg_file");
            }

        }
    }
    pthread_exit(NULL);
}

//This function writes the messages that are destined to user that are currently offline to a file
void write_store_message(message_t msg, string filename)
{
    fstream temporal_msg_file;
    temporal_msg_file.open(filename, fstream::out | fstream::app);

    if (!temporal_msg_file.is_open()) //Check if file was created
    {
        printf("Unable to open the file '%s'\n", filename.c_str());
        exit(-1);
    }
    cout << "Client disconnected, storing message..." << endl;
    //File format
    temporal_msg_file << msg.account_from << "\t" << msg.account_to << "\t" << msg.message_len << endl;
    temporal_msg_file << msg.message << endl;

    temporal_msg_file.close();
}

//Read the user messages from the offline-user messages file and returns the message
//Returns a null message and a 0 if no message was found for the client
pair<message_t, int> read_stored_message(char connected_client[])
{
    message_t message;
    string filename = "temporal_msg_file";
    FILE * file = NULL;
    file = fopen(filename.c_str(), "r");
    pair<message_t, int> return_val;

    //Search all the file
    while (fscanf(file, "%s\t%s\t%d\n", message.account_from, message.account_to, &message.message_len) != EOF)
    {
        //Get the message
        fgets(message.message, BUFFER_SIZE, file);
        //Check if that message was intended for the user
        if(strncmp(message.account_to, connected_client, BUFFER_SIZE) == 0) {
            //If a message is found for the user send it and eliminate it from file
            delete_msg_from_file(message);
            return_val.first = message;
            return_val.second = 1;
            fclose(file);
            return return_val;
        }
    }
    //No message was found for the user
    return_val.first = message;
    return_val.second = 0;
    fclose(file);
    return return_val;
}

//Function to delete a message once it it sent to the user
void delete_msg_from_file(message_t msg_to_delete)
{
    //Create a temporal file to copy all data
    string filename_temp = "temporal";
    //write_store_message(msg_to_delete, filename_temp);
    message_t message;
    string filename = "temporal_msg_file";
    FILE * file = NULL;
    file = fopen(filename.c_str(), "r");
    //Scan all the file
    while (fscanf(file, "%s\t%s\t%d\n", message.account_from, message.account_to, &message.message_len) != EOF)
    {
        fgets(message.message, BUFFER_SIZE, file);
        //If the program finds the message to delete
        if(strncmp(message.account_to, msg_to_delete.account_to, BUFFER_SIZE) == 0
        && strncmp(message.account_from, msg_to_delete.account_from, BUFFER_SIZE) == 0
        && strncmp(message.message, msg_to_delete.message, BUFFER_SIZE) == 0)
        {
            cout << "deleting record... "<< endl;

        } else{
            write_store_message(message, filename_temp); //Write the remaining to the file
        }
    }
    rename(filename_temp.c_str(), filename.c_str()); //Rename file to original one

    fclose(file);
}