// Kevin Crane
//   CS531 - Homework 4
//   keygen.c

#include "keygen.h"


const char* ALPHA = "abcdefghijklmnopqrstuvwxyz\0";
const int ALPHA_LEN = 26;
const int KEY_LEN = 50;

// Generate a keyfile with a specified period
void keygen(int period) {

	char key[KEY_LEN];
	
	// Make sure period is valid size
	if(period < 1) {
		fprintf(stderr, "ERROR: period (-t) must be greater than 0\n");
		return;
	}
	
	
	// Generate random key, seeded from the current time
	srand(time(NULL));
	for (int i = 0; i<KEY_LEN; i++) {
		key[i] = ALPHA[rand() %ALPHA_LEN];
	}
	
	// Print rearranged alphabet 'period' times
	char curr_row[ALPHA_LEN];
	for(int i=0; i<period; i++) {
		int next = 0;
		char temp_char;
		strcpy(curr_row, ALPHA);		// Initialize current row
		
		// Iterate through each letter, choose random letter and swap them
		for(int j=0; j<ALPHA_LEN; j++) {
			next = (next + curr_row[j] + key[(i*ALPHA_LEN + j) %KEY_LEN]) %ALPHA_LEN;
			temp_char = curr_row[j];
			curr_row[j] = curr_row[next];
			curr_row[next] = temp_char;
		}
		printf("%s\n", curr_row);
	}
	
	// PSEUDO-CODE
	// - Generate a random mini-key, seeded from the current time
	// - Iterate through each letter of alphabet:
	//   - Choose next index based off letters in the key
	//   - Swap the current letter with the index made above
	// - Print the current shuffled alphabet
	// - Repeat until 'period' rows have been printed
}
