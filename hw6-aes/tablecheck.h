// Kevin Crane
//   CS531 - Homework 6
//   tablecheck.h

#ifndef TABLECHECK_H
#define TABLECHECK_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "modprod.h"


// Returns true if ch is a hex character (0-9, a-f)
bool is_hex_char(char ch);

// Line beginning with 'S=' should contain a permutation of bytevals 00 to ff, inclusive
bool check_s(FILE* table);

// Lines beginning with 'P=' and 'INVP=' should each contain 8 hex chars, corresponding to 4 bytes
//	Each byte corresponds to the coefficient of a polynomial (x^3 + .. + x^0)
bool check_p(FILE* table);

// Check the integrity of tablefile
bool tablecheck(FILE *table);

#endif
