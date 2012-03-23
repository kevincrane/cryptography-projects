// Kevin Crane
//   CS531 - Homework 4
//   histo.c

#include "histo.h"


// Print character counts, formatted and in order
//	NOTE: used slow, selection sort algorithm because list is guaranteed small, 
//	implementation is simple, and it requires no extra space or data structures
void print_in_order(int char_count[ALPHA_LEN], int tot_count) {
	
	int max_count = -1;
	int max_index = -1;
	printf("L=%d\n", tot_count);
	for(int i=0; i<ALPHA_LEN; i++) {
		
		// Find the next biggest character count
		for(int j=0; j<ALPHA_LEN; j++) {
			if(char_count[j] > max_count) {
				max_count = char_count[j];
				max_index = j;
			}
		}
		
		// Print the biggest char count from this round
		printf("%c: %d (%.2f%%)\n", 'a' + max_index, max_count, ((double)(max_count*100) / tot_count));
		
		// Reset max values and remove biggest char from future consideration
		char_count[max_index] = -1;
		max_count = -1;
		max_index = -1;
	}
}

// Produce a histogram of ciphertext character frequencies in a given period and index
void histo(int per, int index, FILE *text) {

	int tot_count = 0;
	int char_count[ALPHA_LEN];

	// Make sure index and period are valid
	if(per < 1 || index < 1) {
		fprintf(stderr, "ERROR: period and index must both be at least 1.\n");
		return;
	} else if(index > per) {
		fprintf(stderr, "ERROR: index must be less than or equal to period.\n");
		return;
	}
	
	// Initialize character counts
	for(int i=0; i<ALPHA_LEN; i++) {
		char_count[i] = 0;
	}
	
	char* curr_in = (char*)malloc(per);
	fread(curr_in, 1, index-1, text);
	
	// Read all characters from input and add them to char count
	while(fread(curr_in, 1, per, text)) {		
		// Only read letters
		if(*curr_in >= 'a' && *curr_in <= 'z') {
			tot_count++;
			char_count[*curr_in - 'a']++;
		}
	}

	// Print output	
	print_in_order(char_count, tot_count);
	free(curr_in);
	
	// PSEUDO-CODE
	// - Seek to first character to read
	// - Read 'period' bytes from file, 
	// - Take first letter read and add to char count
	// - Use basic selection sort to pick out the most frequent letters
	// - Print the letters and count in order
}
