// Kevin Crane
//   CS531 - Homework 5
//   decrypt.h

#ifndef DECRYPT_H
#define DECRYPT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "encrypt.h"


// Encrypt a single 8-byte block of plaintext using the DES algorithm and the 16 round keys
//	Stores result in cipher_block
void decrypt_block(unsigned char* input_block, unsigned char* cipher_block, 
		unsigned char keys[16][6], FILE* table, bool first);

// Decrypts the input using DES driven by tablefile in the ECB mode using key 
//	as the encryption key (8-byte long and in hexstring format)
void decrypt(char *key, FILE *table, FILE *input);

#endif
