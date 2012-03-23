// Kevin Crane
//   CS531 - Homework 5
//   encrypt3.h

#ifndef ENCRYPT3_H
#define ENCRYPT3_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "encrypt.h"
#include "decrypt.h"

// Encrypts the input using EDE triple DES driven by tablefile in the ECB mode 
//	using key3 as the encryption key (24-byte long and in hexstring format)
void encrypt3(char *key, FILE *table, FILE *input);

#endif
