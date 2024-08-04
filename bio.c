#include "bio.h"

#define PBKDF2_ITER_DEFAULT 10000
#define ERR_EVP_CIPHER_INIT -1
#define ERR_EVP_CIPHER_UPDATE -2
#define ERR_EVP_CIPHER_FINAL -3
#define ERR_EVP_CTX_NEW -4
#define BUFSIZE 1024

int mem_encrypt_decrypt(cipher_params_t *params, unsigned char *enc_json, int enc_json_len, unsigned char *dec_buf, int dec_buf_len) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL) {
        return -1;
    }

    if (!EVP_CipherInit_ex(ctx, params->cipher_type, NULL, params->key, params->iv, params->encrypt)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    int out_len;
    if (!EVP_CipherUpdate(ctx, dec_buf, &out_len, enc_json, enc_json_len)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    int final_out_len;
    if (!EVP_CipherFinal_ex(ctx, dec_buf + out_len, &final_out_len)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    EVP_CIPHER_CTX_free(ctx);
    return out_len + final_out_len;
}

int get_key_iv(const char *password, const EVP_CIPHER *cipher, unsigned char *key, unsigned char *iv) {
    unsigned int i = 0;
    int iter = PBKDF2_ITER_DEFAULT;
    const EVP_MD *digest = EVP_sha256();
    unsigned char keyiv[EVP_MAX_KEY_LENGTH + EVP_MAX_IV_LENGTH];
    int key_len = EVP_CIPHER_key_length(cipher);
    int iv_len = EVP_CIPHER_iv_length(cipher);
    if (PKCS5_PBKDF2_HMAC(password, -1, NULL, 0, iter, digest, key_len + iv_len, keyiv)) {
        while (i < key_len) {
            key[i] = keyiv[i];
            i++;
        }
        while (i < key_len + iv_len) {
            iv[i - key_len] = keyiv[i];
            i++;
        }
        return 0;
    }
    return -1;
}

char* bio_decrypt_json(const char *enc_json, int enc_json_len, const char *password, int *dec_json_len) {
    unsigned char key[EVP_MAX_KEY_LENGTH], iv[EVP_MAX_IV_LENGTH];
    const EVP_CIPHER *cipher_type = EVP_aes_256_cbc();
    cipher_params_t params = {key, iv, 0, cipher_type};

    if (get_key_iv(password, cipher_type, params.key, params.iv)) {
        return NULL;
    }

    int dec_buf_len = enc_json_len + BUFSIZE;

    char *dec_buf = malloc(dec_buf_len);

    int rdec_buf_len = mem_encrypt_decrypt(&params, (unsigned char *)enc_json, enc_json_len, (unsigned char *)dec_buf, dec_buf_len);
    if (rdec_buf_len < 0) {
        free(dec_buf);
        return NULL;
    }

    *dec_json_len = rdec_buf_len;
    return dec_buf;
}
