// Kevin Crane
//   CS531 - Homework 4
//   crypt.c

#include "crypt.h"


const int ALPHA_LEN = 26 + 1;	// The +1 accounts for the \n on each line

// Encrypts an input ASCII text file with the keys specified in keyfile
void crypt(FILE *keyfile, FILE *text) {

	// Determine how long the keyfile is
	fseek(keyfile, 0, SEEK_END);
	int len = ftell(keyfile);
	fseek(keyfile, 0, SEEK_SET);
	
	// Read keyfile and buffer and quickly check its integrity
	if(len%ALPHA_LEN != 0) {
		fprintf(stderr, "ERROR: invalid Vigenere keyfile.\n");
		return;
	}

	// Read the entirety of the keyfile into a buffer
	char* keys = (char*)malloc(len);
	int rows = len / ALPHA_LEN;
	int count = 0;
	fread(keys, len-1, 1, keyfile);
	
	// Verify that the keyfile has valid contents
	for(int i=0; i<len; i++) {
		if((i+1) %ALPHA_LEN == 0) {
			continue;
		} else if(keys[i] < 'a' || keys[i] > 'z') {
			fprintf(stderr, "ERROR: invalid Vigenere keyfile\n");
			return;
		}
	}
	
	// Iterate through each character in input text
	char* curr_in = (char*)malloc(1);
	while(fread(curr_in, 1, 1, text)) {
	
		// Make sure character is valid
		if((*curr_in < 0x20 || *curr_in > 0x7e) && *curr_in != '\n') {
			fprintf(stderr, "ERROR: invalid character (%c) found in input text.\n", *curr_in);
			return;
		}
		
		// Read each chacter and act on it
		if(*curr_in == '\n') {
			// End of file
			printf("\n");
			break;
		} else if(*curr_in >= 'a' && *curr_in <= 'z') {
			// Encrypt char from current permutation function (count %rows)
			// Base Pointer + Scaled Value for correct row + value for which char it is
			printf("%c", *(keys + ((count %rows) * ALPHA_LEN) + (*curr_in - 'a')) );
		} else {
			// Some kind of non-letter character
			printf("%c", *curr_in);
		}
		count++;
	}
	
	free(curr_in);
	
	// PSEUDO-CODE
	// - Seek to end of keyfile to determine how long it is
	// - Verify that the keyfile has a valid number of characters in it
	// - Read keyfile into buffer and verify its contents
	// - Iterate through the input text, char by char
	// - Apply correct permuation function to the char if necessary
	// - Increment to next perm. function and repeat for all chars
}
