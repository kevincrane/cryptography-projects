// Kevin Crane
//   CS531 - Homework 6
//   decrypt.h

#ifndef DECRYPT_H
#define DECRYPT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "encrypt.h"


// Decrypts the input using AES driven by tablefile in the ECB mode using key 
//	as the encryption key (16-byte long and in hexstring format)
void decrypt(char *key, FILE *table, FILE *input);

#endif
