//
// Created by rodrigo on 4/05/19.
//

#include "chat_client.h"


int interrupt_exit = 0;

int main(int argc, char * argv[])
{
    int connection_fd;
    int poll_response;
    char buffer[BUFFER_SIZE];
    char num_cliente[BUFFER_SIZE];
    int timeout = 500;

    connection_fd = connectSocket(argv[1], argv[2]);
    cout << "Ingrese su correo" << endl;
    cin >> num_cliente;
    strcpy(buffer, num_cliente);
    cout << buffer << " connected to server" << endl;
    sendString(connection_fd, buffer, BUFFER_SIZE);
    message_t income_msg;

    pthread_t new_tid;
    thread_data_t * connection_data = NULL;
    connection_data = new thread_data_t;
    strcpy(connection_data->client_id, buffer);
    connection_data->connection_fd = connection_fd;
    if (pthread_create(&new_tid, NULL, client_write, connection_data) == 0)
        printf("Thread created\n");

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
        // Receive the request
        else if (poll_response > 0)
        {
            if (recvString(connection_fd, &income_msg, sizeof(message_t)) == 0)
            {
                cout << "Server unavailable" << endl;
                break;
            }
            unsigned char ciphertext[BUFFER_SIZE];
            unsigned char decryptedtext[BUFFER_SIZE];
            int decrypted_len;
            memcpy((char*)ciphertext,income_msg.message,BUFFER_SIZE);
            decrypted_len = decrypt_msg(ciphertext, income_msg.message_len, decryptedtext);
            decryptedtext[decrypted_len] = '\0';
            //income_msg = decrypt_msg(income_msg);
            cout << "Message from: " << income_msg.account_from << endl;
            cout << "Message: " << (char*) decryptedtext << endl;

        }
    }
    close(connection_fd);

    return 0;
}

void* client_write(void* arg)
{
    auto * data = (thread_data_t*)arg;
    char new_msg[BUFFER_SIZE];
    char msg_dest[BUFFER_SIZE];
    message_t msg;
    strcpy(msg.account_from, data->client_id);
    cin.ignore();

    while(!interrupt_exit)
    {
        cout << "Enter destination: " << endl;
        //cin.ignore();
        cin.getline(msg_dest, sizeof(msg_dest));

        cout << "Enter msg: " << endl;
        cin.getline(new_msg, sizeof(new_msg));

        strcpy(msg.account_to, msg_dest);
        strcpy(msg.message, new_msg);
        int ciphertext_len;
        unsigned char ciphertext[BUFFER_SIZE];

        /* Buffer for the decrypted text */
        unsigned char plaintext[BUFFER_SIZE];

        memcpy((char*)plaintext,msg.message,BUFFER_SIZE);
        ciphertext_len = encrypt_msg(plaintext, strlen((char*)plaintext), ciphertext);
        memcpy(msg.message, (char*)ciphertext, BUFFER_SIZE);
        msg.message_len = ciphertext_len;

        sendString(data->connection_fd, &msg, sizeof(message_t));
    }

    pthread_exit(NULL);
}

int encrypt_msg(unsigned char *plaintext, int plaintext_len, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;

    /* A 256 bit key */
    unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

    /* A 128 bit IV */
    unsigned char *iv = (unsigned char *)"0123456789012345";

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        fatalError("Encrypt_error");

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        fatalError("Encrypt_error");

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        fatalError("Encrypt_error");
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        fatalError("Encrypt_error");
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt_msg(unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext)
{
    /* A 256 bit key */
    unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

    /* A 128 bit IV */
    unsigned char *iv = (unsigned char *)"0123456789012345";

    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        fatalError("Decrypt_error1");

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        fatalError("Decrypt_error2");

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        fatalError("Decrypt_error3");
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        fatalError("Decrypt_error4");
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}