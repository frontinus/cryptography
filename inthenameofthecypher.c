#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

#define BUFFER_SIZE 4096

int main(int argc, char* argv[]) {
    // Check the number of parameters
    if (argc != 6) {
        printf("Usage: %s inputfile key iv outputfile algorithm\n", argv[0]);
        return 1;
    }

    // Read parameters
    char* inputfile = argv[1];
    char* key_str = argv[2];
    char* iv_str = argv[3];
    char* outputfile = argv[4];
    char* algorithm = argv[5];

    // Open input file
    FILE* fin = fopen(inputfile, "rb");
    if (!fin) {
        printf("Could not open input file\n");
        return 1;
    }

    // Open output file
    FILE* fout = fopen(outputfile, "wb");
    if (!fout) {
        printf("Could not open output file\n");
        return 1;
    }

    // Convert key and iv to bytes
    unsigned char* key = (unsigned char*)key_str;
    unsigned char* iv = (unsigned char*)iv_str;

    // Initialize encryption context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        printf("Could not create encryption context\n");
        return 1;
    }

    // Initialize encryption
    if (!EVP_EncryptInit_ex(ctx, EVP_get_cipherbyname(algorithm), NULL, key, iv)) {
        printf("Could not initialize encryption\n");
        return 1;
    }

    // Read input file and write encrypted data to output file
    unsigned char inbuf[BUFFER_SIZE], outbuf[BUFFER_SIZE + EVP_MAX_BLOCK_LENGTH];
    int bytes_read, bytes_written, total_bytes_written = 0;
    while ((bytes_read = fread(inbuf, 1, BUFFER_SIZE, fin)) > 0) {
        if (!EVP_EncryptUpdate(ctx, outbuf, &bytes_written, inbuf, bytes_read)) {
            printf("Could not encrypt data\n");
            return 1;
        }
        fwrite(outbuf, 1, bytes_written, fout);
        total_bytes_written += bytes_written;
    }

    // Finalize encryption
    if (!EVP_EncryptFinal_ex(ctx, outbuf, &bytes_written)) {
        printf("Could not finalize encryption\n");
        return 1;
    }
    fwrite(outbuf, 1, bytes_written, fout);
    total_bytes_written += bytes_written;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);
    fclose(fin);
    fclose(fout);

    printf("Encryption successful. Encrypted %d bytes.\n", total_bytes_written);

    return 0;
}
