#include <openssl/evp.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define AES_128_KEY_SIZE 16

int hexToInt(char c)
{
    int first = c / 16 - 3;
    int second = c % 16;
    int result = first*10 + second;
    if (result >9) result--;
    return result;
}

int HexToChar(char c, char d)
{
    int high = hexToInt(c) * 16;
    int low = hexToInt(d);
    return high + low;
}

void fillKey(char * key, const int size){
    if (size == AES_128_KEY_SIZE){
        key[AES_128_KEY_SIZE] = '\0';
        return;
    }
    for (int i = size; i < AES_128_KEY_SIZE; ++i){
        key[i] = ' ';
    }
    key[AES_128_KEY_SIZE] = '\0';
}

void capitalize(char * str, const int size){
    for (int i = 0; i < size; ++i){
        if(str[i] >= 'a' && str[i] <= 'z') str[i] = str[i] - 32;
    }
}

int main()
{
    unsigned char outbuf[1024]; // buffer used to store the encrypted message
    int found = 0; // flag to check if the cipher was found

    char plaintext[] = "This is a top secret."; // given message 
    char ciphertext[] = "8d20e5056a8d24d0462ce74e4904c1b513e10d1df4a2ef2ad4540fae1ca0aaf9"; // given ciphertext
    unsigned char iv[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,'\0'};

    // capitalize al the letters in the ciphertext
    capitalize(ciphertext, strlen(ciphertext));

    char buf = 0;

    // convert the ciphertext from hex to ascii
    unsigned char cipher[1024];
    int k = 0;
    for (int i = 0; i < strlen(ciphertext); ++i)
    {
        if (i % 2 == 1){
            cipher[k] = HexToChar(buf, ciphertext[i]);
            k++;
        }
        else buf = ciphertext[i];
    }
    cipher[k] = '\0';

    FILE * fp = fopen("words.txt", "r");
    if(!fp){
        printf("Cannot open words.txt\n");
        exit(EXIT_FAILURE);
    }
    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);
    unsigned char key[AES_128_KEY_SIZE];
    int outlen, tmplen;
    while(fgets(key, sizeof(key), fp)){
        // fill the key with the appropriate spaces
        fillKey(key,strlen(key)-1);

        // begin the encryption using the key and iv
        EVP_EncryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL, key, iv);

        // check the evp flags
        if(!EVP_EncryptUpdate(&ctx, outbuf, &outlen, plaintext, strlen(plaintext))) return -1;

        if(!EVP_EncryptFinal_ex(&ctx, outbuf + outlen, &tmplen)) return -1;

        outlen += tmplen;
        EVP_CIPHER_CTX_cleanup(&ctx);

        // compare the given cipher with the one gotten from the encryption at each key
        if(strcmp(cipher,outbuf) == 0){
            printf("The Used Key is:  %s\n",key);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if(!found){
        printf("Key cannot be found\n");
    }
    return 0;
}