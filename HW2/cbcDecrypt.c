#include <openssl/evp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AES_128_KEY_SIZE 16

void makeKey(char * str, const int size){
	for (int i = size; i < AES_128_KEY_SIZE; ++i){
		str[i] = ' ';
	}
	str[AES_128_KEY_SIZE+1] = '\0';
}

int main(int argc, char argv[])
{
	//aes-128-cbc encryption
	// numbers in the IV are all 0s
	// use word.txt for the list of possible keys
	unsigned char plaintext[] = "This is a top secret.";
	unsigned char cipherText[] = "8d20e5056a8d24d0462ce74e4904c1b513e10d1df4a2ef2ad4540fae1ca0aaf9";
	unsigned char iv[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,'\0'};

	
	FILE * fp = fopen("words.txt","r");
	if(!fp){
		perror("Unable to open words.txt\n");
		exit(EXIT_FAILURE);
	}
	char key[AES_128_KEY_SIZE+1]; // needs an extra space for the \0 char
	while(fgets(key, AES_128_KEY_SIZE+1, fp)){
		EVP_CIPHER_CTX * ctx =  EVP_CIPHER_CTX_new();
		makeKey(key,strlen(key)-1);
		unsigned char outbuf[1024];
		int outlen, tmplen;
		EVP_CIPHER_CTX_init(ctx);
		EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
		if (!EVP_EncryptUpdate(ctx, outbuf, &outlen, plaintext, strlen(plaintext))){
			perror("Error with cypher\n");
			EVP_CIPHER_CTX_free(ctx);
			return -1;
		}
		if (!EVP_EncryptFinal_ex(ctx, outbuf + outlen, &tmplen)) {
			EVP_CIPHER_CTX_free(ctx);
			return -1;
    	}
		outlen += tmplen;
		EVP_CIPHER_CTX_free(ctx);
		// make the string into hexa string
		char * hex_str = (char*) malloc((outlen * 2) + 1);
		for (int i = 0; i < outlen; ++i){
			sprintf(&hex_str[i * 2], "%02x", outbuf[i]);
		}
		if (strcmp(cipherText,hex_str) == 0){
			printf("Key used is: %s\n",key);
			return 0;
		}
	}
	fclose(fp);
	return 0;
}
