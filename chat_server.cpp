//
// Created by rodrigo on 2/05/19.
//

#include "chat_server.h"


int interrupt_exit = 0;
map<string, int>connected_users;

int main() {
    printLocalIPs();
    int server = initServer(const_cast<char *>("8989"), MAX_QUEUE);
    //std::cout << server << std::endl;
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
        //Continue bank operations
        if (poll_response > 0)
        {
            if (test_fds[0].revents & POLLIN)
            {
                // ACCEPT
                // Wait for a client connection
                client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_size);
                if (client_fd == -1)
                {
                    fatalError("ERROR: accept");
                }

                // Get the data from the client
                inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
                printf("Received incomming connection from %s on port %d\n", client_presentation, client_address.sin_port);
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
    int timeout = 500;
    auto * data = (thread_data_t*)arg;
    char buffer[BUFFER_SIZE];
    recvString(data->connection_fd, buffer, BUFFER_SIZE);
    data->client_id = buffer;
    connected_users.insert(pair<string, int>(buffer, data->connection_fd));
    pair<message_t, int> now_connected_data;
    now_connected_data = read_stored_message(buffer);
    cout << now_connected_data.second << endl;
    while (now_connected_data.second == 1)
    {
        cout << "User has now logged in, sending message" << endl;
        cout << now_connected_data.first.message << endl;
        sendString(connected_users.find(now_connected_data.first.account_to)->second, &now_connected_data.first,sizeof(message_t));
        now_connected_data = read_stored_message(buffer);
    }
    /*if (now_connected_data.second == 1)
    {
        cout << "User has now logged in, sending message" << endl;
        cout << now_connected_data.first.message << endl;
    }*/
    message_t msg;

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
        /*else if (poll_response == 0)
        {
            cout << "." << endl;
        }*/
        // Receive the request
        else if (poll_response > 0)
        {
            if (recvString(data->connection_fd, &msg, sizeof(message_t)) == 0)
            {
                cout << "Client disconnected" << endl;
                connected_users.erase(data->client_id);
                break;
            }

            /*cout << "Account from: "<< msg.account_from << endl;
            cout << "Account to: "<< msg.account_to << endl;
            cout << "Mensaje: "<< msg.message << endl;*/

            /*map<string, int>::iterator it;
            for (it = connected_users.begin(); it != connected_users.end(); it++)
            {
                std::cout << it->first << ' ' << it->second << '\n';
            }*/
            if (connected_users.find(msg.account_to)->second != 0)
                sendString(connected_users.find(msg.account_to)->second, &msg,sizeof(message_t));
            else if (connected_users.find(msg.account_to)->second == 0)
            {
                write_store_message(msg, "temporal_msg_file");
            }

        }
    }
    pthread_exit(NULL);
}

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
    //Write image header

    temporal_msg_file << msg.account_from << "\t" << msg.account_to << endl;
    //fprintf (temporal_msg_file, "%s\t%s\n", msg.account_from, msg.account_to);
    //fputs(strcat(msg.message,"\n"), temporal_msg_file);
    temporal_msg_file << msg.message << endl;

    temporal_msg_file.close();
}

pair<message_t, int> read_stored_message(char connected_client[])
{
    message_t message;
    string filename = "temporal_msg_file";
    FILE * file = NULL;
    file = fopen(filename.c_str(), "r");
    pair<message_t, int> return_val;

    while (fscanf(file, "%s\t%s\n", message.account_from, message.account_to) != EOF)
    {
        fgets(message.message, BUFFER_SIZE, file);
        //printf("%s\t%s\n%s\n", message1, message2, message3);


        if(strncmp(message.account_to, connected_client, BUFFER_SIZE) == 0) {
            delete_msg_from_file(message);
            return_val.first = message;
            return_val.second = 1;
            return return_val;
            break;
        }
    }
    return_val.first = message;
    return_val.second = 0;
    fclose(file);
    return return_val;
}

void delete_msg_from_file(message_t msg_to_delete)
{
    string filename_temp = "temporal";
    //write_store_message(msg_to_delete, filename_temp);
    message_t message;
    string filename = "temporal_msg_file";
    FILE * file = NULL;
    file = fopen(filename.c_str(), "r");

    while (fscanf(file, "%s\t%s\n", message.account_from, message.account_to) != EOF)
    {
        fgets(message.message, BUFFER_SIZE, file);
        cout << "Current message: " << message.account_from << message.account_to << message.message << endl;
        cout << "Message to delete: " << msg_to_delete.account_from << msg_to_delete.account_to << msg_to_delete.message << endl;
        if(strncmp(message.account_to, msg_to_delete.account_to, BUFFER_SIZE) == 0
        && strncmp(message.account_from, msg_to_delete.account_from, BUFFER_SIZE) == 0
        && strncmp(message.message, msg_to_delete.message, BUFFER_SIZE) == 0)
        {
            cout << "deleting record "<< endl;

        } else{
            write_store_message(message, filename_temp);
        }
    }
    rename(filename_temp.c_str(), filename.c_str());

    fclose(file);
}