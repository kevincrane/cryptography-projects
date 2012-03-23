// Kevin Crane
//   CS531 - Homework 4
//   invkey.c

#include "invkey.h"


const int ALPHA_LEN = 26;

// Create an inverse permutation key file from keyfile
void invkey(FILE *in) {
	
	char curr_row[ALPHA_LEN+1];
	char out_row[ALPHA_LEN+1];
	
	// Read through the whole file, line by line
	while(fgets(curr_row, ALPHA_LEN+5, in)) {
		// Make sure the row is a valid length
		if(strlen(curr_row) != (ALPHA_LEN+1)) {
			fprintf(stderr, "ERROR: invalid row found in keyfile.\n");
			return;
		}
		
		// Iterate through characters in row
		for(int i=0; i<ALPHA_LEN; i++) {
			// Make sure the row is all lowercase chars
			if(curr_row[i] < 'a' || curr_row[i] > 'z') {
				fprintf(stderr, "ERROR: invalid characters found in keyfile.\n");
				return;
			}
			
			// Set correct character in inverse row to the next letter
			out_row[curr_row[i] - 'a'] = 'a' + i;
		}
		out_row[ALPHA_LEN] = '\0';
		
		printf("%s\n", out_row);
	}
	
	// PSEUDO-CODE
	// - Read in a text file, row by row
	// - Read a character from that row
	// - Identify what position that char should be in, write next letter there
	// - Repeat with all chars in all rows
}
