//
// Created by rodrigo on 10/05/19.
//

#ifndef PROYECTOFINALPROGAV_ENCRYPTION_H
#define PROYECTOFINALPROGAV_ENCRYPTION_H

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include "fatal_error.h"

int encrypt_msg(unsigned char *plaintext, int plaintext_len, unsigned char *ciphertext);

int decrypt_msg(unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext);

#endif //PROYECTOFINALPROGAV_ENCRYPTION_H
