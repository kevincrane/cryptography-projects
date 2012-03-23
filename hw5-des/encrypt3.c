// Kevin Crane
//   CS531 - Homework 5
//   encrypt3.c

#include "encrypt3.h"


// Encrypts the input using EDE triple DES driven by tablefile in the ECB mode 
//	using key3 as the encryption key (24-byte long and in hexstring format)
void encrypt3(char *key, FILE *table, FILE *input) {
	
	char* init_key1 = (char*)malloc(16);
	char* init_key2 = (char*)malloc(16);
	char* init_key3 = (char*)malloc(16);
	unsigned char keys1[16][6] = { { 0 } };
	unsigned char keys2[16][6] = { { 0 } };
	unsigned char keys3[16][6] = { { 0 } };
	unsigned char* input_block = (unsigned char*)calloc(1,8);
	unsigned char* cipher_block = (unsigned char*)calloc(1,8);
	
	// Verify key length
	if(strlen(key) != 48) {
		fprintf(stderr, "ERROR: invalid key length. Must be 48 hex characters.\n");
		return;
	}
	
	// Verify table
	if(!tablecheck(table)) {
		return;
	}
	
	// Split initial 24-byte key into 3 8-byte keys
	memcpy(init_key1, key, 16);
	memcpy(init_key2, key+16, 16);
	memcpy(init_key3, key+32, 16);
	
	// Generate round keys for all 3 keys
	if(!(gen_keys(init_key1, table, keys1) && (gen_keys(init_key2, table, keys2)) 
			&& (gen_keys(init_key3, table, keys3)))) {
		fprintf(stderr, "ERROR: did not properly generate round keys.\n");
		return;
	}
	
	// Encrypt everything from input in 8 byte-block
	bool first = false;
	int size;
	while((size = fread(input_block, 1, 8, input))) {
		if((*input_block == '\n') && (size == 1))
			continue;
		
		// Decrypt a single block
		encrypt_block(input_block, cipher_block, keys1, table, first);
		
		for(int i=0; i<8; i++) input_block[i] = 0x00;
		decrypt_block(cipher_block, input_block, keys2, table, first);
		
		for(int i=0; i<8; i++) cipher_block[i] = 0x00;
		encrypt_block(input_block, cipher_block, keys3, table, first);
		
		// Print current cipher block values to output
		printf("%c%c%c%c%c%c%c%c",
				*cipher_block, *(cipher_block+1), *(cipher_block+2), *(cipher_block+3), 
				*(cipher_block+4), *(cipher_block+5), *(cipher_block+6), *(cipher_block+7) );
		
		// Reset blocks for next iteration
		first = false;
		for(int i=0; i<8; i++) {
			input_block[i] = 0x00;
			cipher_block[i] = 0x00;
		}
	}
	
	free(init_key1);
	free(init_key2);
	free(init_key3);
	free(input_block);
	free(cipher_block);

	
	// PSEUDO-CODE
	// - Break original key in thirds into 3 new keys
	// - Generate round keys for each of these 3 keys
	// - Break original input into 8-byte blocks
	// - Decrypt, encrypt, decrypt these blocks using keys 3, 2, and 1 respectively
	// - Print output, repeat
}
