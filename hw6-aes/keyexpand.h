// Kevin Crane
//   CS531 - Homework 6
//   keyexpand.h

#ifndef KEYEXPAND_H
#define KEYEXPAND_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tablecheck.h"


// Initialize S-Box variable and store in byte array s_box
void init_s_box(unsigned char* s_box, FILE* table);

// Initialize polynomial from tablefile
void init_poly(unsigned char* poly, FILE* table, const char* header);

// Initialize round Rijndael constant and store in array rcon
void init_rcon(unsigned char rcon[256]);

// Perform the key expansion for AES-128 using 16-byte key; each round stored in out_key
void key_expansion(unsigned char init_key[16], unsigned char out_key[44][4], FILE* table);

// Perform the key expansion for AES-128 using key as 16-byte string in hexstring format
void keyexpand(char* key_chars, FILE* table);

#endif
