// Kevin Crane
//   CS531 - Homework 5
//   decrypt.c

#include "decrypt.h"


// Encrypt a single 8-byte block of plaintext using the DES algorithm and the 16 round keys
//	Stores result in cipher_block
void decrypt_block(unsigned char* input_block, unsigned char* cipher_block, unsigned char keys[16][6], FILE* table, bool first) {

	unsigned char inv_keys[16][6] = { { 0 } };
	
	// Reverse keys for 
	for(int i=0; i<16; i++) {
		memcpy(inv_keys[i], keys[15-i], 6);
	}
	
	// Perform DES encryption on this 8-byte block
	encrypt_block(input_block, cipher_block, inv_keys, table, first);
}


// Decrypts the input using DES driven by tablefile in the ECB mode using key 
//	as the encryption key (8-byte long and in hexstring format)
void decrypt(char *key, FILE *table, FILE *input) {
	
	unsigned char keys[16][6] = { { 0 } };
	unsigned char* input_block = (unsigned char*)calloc(1,8);
	unsigned char* cipher_block = (unsigned char*)calloc(1,8);
	
	// Verify table
	if(!tablecheck(table)) {
		return;
	}
	
	// Generate 16 round keys
	if(!gen_keys(key, table, keys)) {
		fprintf(stderr, "ERROR: did not properly generate round keys.\n");
		return;
	}
	
	// Make sure the user didn't use a weak key
	if(is_weak_key(keys)) {
		fprintf(stderr, "ERROR: the key provided is weak (palindromic) and easily broken.\n");
		return;
	}
	
	// Decrypt everything from input in 8 byte-block
	bool first = true;
	int size;
	while((size = fread(input_block, 1, 8, input))) {
		if((*input_block == '\n') && (size == 1))
			continue;
		
		// Decrypt a single block
		decrypt_block(input_block, cipher_block, keys, table, first);
		
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
	
	free(input_block);
	free(cipher_block);
	
	// PSEUDO-CODE
	// - Generate 16 round keys
	// - Read file input in 8-byte blocks
	// - Perform the same operations as encryption
	//   - Only difference, before each round, reverse the order of the keys
	// - Print each block output
	// - Repeat
}
