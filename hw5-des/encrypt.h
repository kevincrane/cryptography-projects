// Kevin Crane
//   CS531 - Homework 5
//   encrypt.h


#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tablecheck.h"


// Generate an array of 16 48-bit keys from initial 64-bit key
bool gen_keys(char *init_key, FILE *table, unsigned char keys[16][6]);

// Returns true if the round keys are weak (palindromic), false otherwise
bool is_weak_key(unsigned char keys[16][6]);

// Encrypt a single 8-byte block of plaintext using the DES algorithm and the 16 round keys
//	Stores result in cipher_block
void encrypt_block(unsigned char* input, unsigned char* cipher_block, unsigned char keys[16][6], FILE* table, bool first);

// Encrypts the input using DES driven by tablefile in the ECB mode using key 
//	as the encryption key (8-byte long and in hexstring format)
void encrypt(char *key, FILE *table, FILE *input);

#endif
