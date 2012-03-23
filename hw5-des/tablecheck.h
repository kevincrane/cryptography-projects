// Kevin Crane
//   CS531 - Homework 5
//   tablecheck.h

#ifndef TABLECHECK_H
#define TABLECHECK_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Check given int array to determine if IP (Initial Permutation) values are valid
//	Permutation of values 1 to 64 inclusive, must appear once each
bool check_ip(FILE* table);

// Check given int array to determine if E (Expansion) values are valid
//	48 values, 1 to 32 inclusive; must include once or twice each
bool check_e(FILE* table);

// Check given int array to determine if S# (Substitution) values are valid
//	64 values in 4 groups, permutation of 0 to 15 inclusive in each group
bool check_s(FILE* table, char* header);

// Check given int array to determine if V (somethng?) values are valid
//	16 values; should have 12 2's and 4 1's
bool check_v(FILE* table);

// Check given int array to determine if PC1 (Permuted Choice 1) values are valid
//	Permutation of values 1 to 64 inclusive, multiples of 8 removed
bool check_pc1(FILE* table);

// Check given int array to determine if PC2 (Permuted Choice 2) values are valid
//	Permutation of values 1 to 56 inclusive, 8 random values removed
bool check_pc2(FILE* table);


// Search the input file for the row that starts with the header string, stores vals in array
bool read_line_vals(FILE *fin, char* header, int* vals, int expected_val);

// Check the integrity of tablefile
bool tablecheck(FILE *table);

#endif
