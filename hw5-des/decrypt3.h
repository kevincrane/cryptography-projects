// Kevin Crane
//   CS531 - Homework 5
//   decrypt3.h

#ifndef DECRYPT3_H
#define DECRYPT3_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "encrypt.h"
#include "decrypt.h"

// Encrypts the input using EDE triple DES driven by tablefile in the ECB mode 
//	using key3 as the encryption key (24-byte long and in hexstring format)
void decrypt3(char *key, FILE *table, FILE *input);

#endif
