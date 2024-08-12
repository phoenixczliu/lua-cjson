#include <stdio.h>
#include <openssl/evp.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#define DECRYPT_BUFFER_SIZE 1024

typedef struct cipher_params_s {
    unsigned char *key;
    unsigned char *iv;
    unsigned int encrypt;
    const EVP_CIPHER *cipher_type;
} cipher_params_t;

int bio_decrypt_json(const char *enc_json, int enc_json_len, char *dec_json_buf, int dec_json_buf_len);