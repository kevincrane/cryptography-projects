// Kevin Crane
//   CS531 - Homework 6
//   encrypt.h


#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tablecheck.h"
#include "modprod.h"
#include "keyexpand.h"


// Prints the current contents of the state, plus a line header
void print_state(unsigned char state[4][4], int round, const char* header);


// Adds the current round key to state (4 rows from round_key, 4 cols in each row)
void add_round_key(unsigned char state[4][4], unsigned char round_key[44][4], int round);

// Substitutes each byte in state according to an S-Box
void sub_bytes(unsigned char state[4][4], unsigned char* s_box);

// Shifts each byte in each row to the left by a number of spaces
//	2nd row shifts 1, 3rd shifts 2, 4th row shifts 3
void shift_rows(unsigned char state[4][4]);


// Encrypts the input using AES-128 driven by tablefile; stores in output[4][4]
void encrypt_block(unsigned char input_block[16], unsigned char output_block[16],
		unsigned char round_key[44][4], FILE* table);

// Encrypts the input using AES driven by tablefile in the ECB mode using key 
//	as the encryption key (16-byte long and in hexstring format)
void encrypt(char *key_chars, FILE *table, FILE *input);

#endif
