// Kevin Crane
//   CS531 - Homework 6
//   encrypt.h


#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tablecheck.h"

// Encrypts the input using AES driven by tablefile in the ECB mode using key 
//	as the encryption key (16-byte long and in hexstring format)
void encrypt(char *key, FILE *table, FILE *input);

#endif
