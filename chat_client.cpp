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

#include "chat_client.h"

//Check if program still running
int interrupt_exit = 0;

int main(int argc, char * argv[])
{
    int connection_fd;
    int poll_response;
    char buffer[BUFFER_SIZE];
    //Id del usuario
    char num_cliente[BUFFER_SIZE];
    int timeout = 500;

    connection_fd = connectSocket(argv[1], argv[2]); //conectarse a servidor
    cout << "Ingrese su correo" << endl;
    cin >> num_cliente;
    strcpy(buffer, num_cliente);
    cout << buffer << " connected to server" << endl;
    sendString(connection_fd, buffer, BUFFER_SIZE); //Enviar id del usuario al servidor
    message_t income_msg;
    //Datos que se almacenaran en el thread
    pthread_t new_tid;
    thread_data_t * connection_data = NULL;
    connection_data = new thread_data_t;
    strcpy(connection_data->client_id, buffer);
    connection_data->connection_fd = connection_fd;
    //printf("Thread created\n");
    pthread_create(&new_tid, NULL, client_write, connection_data);
    //While program still running
    while (!interrupt_exit)
    {
        // Create a structure array to hold the file descriptors to poll
        struct pollfd test_fds[1];
        // Fill in the structure
        test_fds[0].fd = connection_fd;
        test_fds[0].events = POLLIN;    // Check for incomming data
        poll_response = poll(test_fds, 1, timeout);

        if (poll_response == -1)
        {
            if (errno == EINTR)
            {
                break;
            }
        }
        // Receive a message
        else if (poll_response > 0)
        {
            if (recvString(connection_fd, &income_msg, sizeof(message_t)) == 0)
            {
                cout << "Server unavailable" << endl;
                break;
            }
            //Decrypt message
            unsigned char ciphertext[BUFFER_SIZE];
            unsigned char decryptedtext[BUFFER_SIZE];
            int decrypted_len;

            memcpy((char*)ciphertext,income_msg.message,BUFFER_SIZE);
            decrypted_len = decrypt_msg(ciphertext, income_msg.message_len, decryptedtext);
            decryptedtext[decrypted_len] = '\0';

            cout << "Message from: " << income_msg.account_from << endl;
            cout << "Message: " << (char*) decryptedtext << endl;

        }
    }
    close(connection_fd);

    return 0;
}

//Thread to manage input messages
void* client_write(void* arg)
{
    //Receive user data
    auto * data = (thread_data_t*)arg;
    char new_msg[BUFFER_SIZE];
    char msg_dest[BUFFER_SIZE];
    message_t msg;
    strcpy(msg.account_from, data->client_id);
    cin.ignore();

    //While program still running
    while(!interrupt_exit)
    {
        cout << "Enter destination: " << endl; //Addressee
        cin.getline(msg_dest, sizeof(msg_dest));

        cout << "Enter message: " << endl;
        cin.getline(new_msg, sizeof(new_msg));

        strcpy(msg.account_to, msg_dest);
        strcpy(msg.message, new_msg);
        //Encrypt message
        int ciphertext_len;
        unsigned char ciphertext[BUFFER_SIZE];

        /* Buffer for the decrypted text */
        unsigned char plaintext[BUFFER_SIZE];

        memcpy((char*)plaintext,msg.message,BUFFER_SIZE);
        ciphertext_len = encrypt_msg(plaintext, strlen((char*)plaintext), ciphertext);
        memcpy(msg.message, (char*)ciphertext, BUFFER_SIZE);
        msg.message_len = ciphertext_len;
        //Send encrypted message
        sendString(data->connection_fd, &msg, sizeof(message_t));
    }

    pthread_exit(NULL);
}