// Kevin Crane
//   CS531 - Homework 6
//   decrypt.h

#ifndef DECRYPT_H
#define DECRYPT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "encrypt.h"


// Prints the current contents of the state, plus a line header
void inv_print_state(unsigned char state[4][4], int round, const char* header);

// Inverts AES's 256-byte S-Box array
void invert_s_box(unsigned char* s_box, unsigned char* inv_s_box);


// Shifts each byte in each row to the right by a number of spaces
//	2nd row shifts 1, 3rd shifts 2, 4th row shifts 3
void inv_shift_rows(unsigned char state[4][4]);


// Decrypts the input using AES-128 driven by tablefile; stores in output[4][4]
void decrypt_block(unsigned char input_block[16], unsigned char output_block[16],
		unsigned char round_key[44][4], FILE* table);

// Decrypts the input using AES driven by tablefile in the ECB mode using key 
//	as the encryption key (16-byte long and in hexstring format)
void decrypt(char *key, FILE *table, FILE *input);

#endif
