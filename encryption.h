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

#ifndef PROYECTOFINALPROGAV_ENCRYPTION_H
#define PROYECTOFINALPROGAV_ENCRYPTION_H

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include "fatal_error.h"

//Function to encrypt message
int encrypt_msg(unsigned char *plaintext, int plaintext_len, unsigned char *ciphertext);
//Function to decrypt
int decrypt_msg(unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext);

#endif //PROYECTOFINALPROGAV_ENCRYPTION_H
