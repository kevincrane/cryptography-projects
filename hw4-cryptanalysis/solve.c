// Kevin Crane
//   CS531 - Homework 4
//   solve.c

#include "solve.h"

const int ALPHA_LEN = 26;
const double kp = 0.0658;


// Return true if character is lowercase letter
bool is_letter(char character) {
	if(character < 'a' || character > 'z')
		return false;
		
	return true;
}


/* ##### KASISKI ANALYSIS ##### */

// Returns true if the word_buf of length len is made up entirely of letters
bool is_only_letters(char* word_buf, int len) {
	for(int i=0; i<len; i++) {
		if(!is_letter(word_buf[i]))
			return false;
	}
	
	return true;
}
 
// Run Kasiski analysis on text, looking for matches of length word_len
bool kasiski_analyze(char* text, int file_len, int word_len) {
	char init_buf[word_len+1];
	char comp_buf[word_len+1];
	bool found_match = false;
	
	// Iterate through entire text file
	for(int i=0; i<file_len-word_len-1; i++) {
		
		// Read initial string to be compared to
		memcpy(init_buf, &text[i], word_len);
		init_buf[word_len] = '\0';

		// If current string isn't only letters, skip it
		if(!is_only_letters(init_buf, word_len)) {
			continue;
		}
		
		// Iterate through remaining letters to compare to init string
		for(int j=i+1; j<file_len-word_len; j++) {
			memcpy(comp_buf, &text[j], word_len);
			comp_buf[word_len] = '\0';
			
			// If current string isn't only letters, skip it
			if(!is_only_letters(comp_buf, word_len)) {
				continue;
			}
			
			// comp_buf matches init_buf!
			if(strcmp(init_buf, comp_buf) == 0) {
				// Print output
				printf("len=%d, i=%d, j=%d, j-i=%d, %s\n", word_len, i, j, j-i, init_buf);
				found_match = true;
			}
		}
	}
	
	if(!found_match) {
		printf("len=%d, no more matches\n", word_len);
	}
	
	return found_match;
}

// Analyze the input text file using the Kasiski method
void kasiski(char *text, int file_len) {

	bool found_match = true;
	int word_len = 4;
	
	// Print header
	printf("Kasiski Method\n");
	printf("==============\n");
	
	// Run kasiski analysis on strings of increasing length
	while(found_match) {
		found_match = kasiski_analyze(text, file_len, word_len);
		word_len++;
	}
}


/*	##### INDEX OF COINCIDENCE #### */

// Return a float value with the 'index of coincidence' for this cipher text
//		Equation: IC=(SUM(i=1:26; f_i*(f_i - 1)) / (L*(L - 1)));	Eq 7.2, pg. 249 of textbook
float calc_ic(int char_count[ALPHA_LEN], int tot_count) {
	float ic = 0;
	
	// Add up (f_i * (f_i - 1)) for all letters in alphabet
	for(int i=0; i<ALPHA_LEN; i++) {
		ic += (char_count[i] * (char_count[i] - 1));
	}
	
	// Divide by (L * (L - 1))
	ic /= (tot_count * (tot_count - 1));
	
	return ic;
}

// Return a float value with the expected 'index of coincidence' for a ciphertext with key of length per
//		Equation: EIC=(kp * (L-t)/(t*(L-1)) + (kr * (t-1)/t * L/(L-1)));	Eq 7.3, pg. 249 of textbook
float calc_eic(int per, int tot_count) {
	float eic = 0;
	eic = ( kp * (tot_count - per) / (per * (tot_count - 1)) );
	eic = eic + 1.0*( ((float)1/ALPHA_LEN) * ((float)(per - 1) / per) * ((float)tot_count / (tot_count - 1)) );
	
	return eic;
}

// Analyze the input text file using the index of coincidence method
void index_of_coincidence(char *text, int file_len, int maxt) {
	
	int char_count[ALPHA_LEN];
	int tot_count = 0;
	
	// Print header
	printf("Average Index of Coincidence\n");
	printf("============================\n");
	
	// Initialize frequency array
	for(int i=0; i<ALPHA_LEN; i++) {
		char_count[i] = 0;
	}
	
	// Read all characters from input and add them to char count
	for(int i=0; i<file_len; i++) {
		// Only read letters
		if(is_letter(text[i])) {
			tot_count++;
			char_count[text[i] - 'a']++;
		}
	}
	
	// Print character counts
	printf("L=%d\n", tot_count);
	for(int i=0; i<ALPHA_LEN; i++) {
		printf("f('%c')=%d\n", 'a' + i, char_count[i]);
	}
	
	// Print index of coincidence for this ciphertext
	printf("IC=%.9f\n", calc_ic(char_count, tot_count));
	
	// Print expected index of coincidence for this ciphertext
	for(int i=1; i<=maxt; i++) {
		if(i != 1) {
			printf("t=%d, E(IC)=%.9f\n", i, calc_eic(i, tot_count));
		} else {
			printf("t=%d, E(IC)=%.4f\n", i, kp);
		}
	}
	
}


/*	##### AUTO-CORRELATION #### */


// Calculate auto-correlation count for text file with given period
void calc_ac(char *text, int file_len, int per) {
	int count = 0;
	
	// Make sure period valid
	if(per < 1 || per > file_len) {
		fprintf(stderr, "ERROR: invalid period size\n");
		return;
	}
	
	// Iterate through entire text file looking for correlated matches
	for(int i=0; i<file_len-per; i++) {
		// If chars at index 'i' and 'i+per' are not letters, skip 'em
		if(!is_letter(text[i]) || !is_letter(text[i+per])) {
			continue;
		}
		// Match found at index 'i' and 'i+per'
		if(text[i] == text[i+per]) {
			count++;
		}
	}
	
	// Output number of correlated matches found
	printf("t=%d, count=%d\n", per, count);
}

// Analyze the input text file using the auto-correlation method
void auto_correlation(char *text, int file_len, int maxt) {
	
	// Print header
	printf("Auto-correlation Method\n");
	printf("=======================\n");
	
	// Calculate the auto-correlation count for each t from 1 to maxt
	for(int i=1; i<=maxt; i++) {
		calc_ac(text, file_len, i);
	}
		
}


/*	##### MAIN SOLVER ##### */
 
// Produce a histogram of ciphertext character frequencies
void solve(int maxt, FILE *in) {

	// Determine how long the keyfile is
	fseek(in, 0, SEEK_END);
	int len = ftell(in);
	fseek(in, 0, SEEK_SET);
	
	// Read the entirety of the text input into a buffer
	char* text = (char*)malloc(len);
	fread(text, len, 1, in);
	
	// Run Kasiski analysis on input text
	kasiski(text, len);
	printf("\n");
	
	// Run index of coincidence analysis on input text
	index_of_coincidence(text, len, maxt);
	printf("\n");

	// Run auto-correlation analysis on input text
	auto_correlation(text, len, maxt);
	printf("\n");
	
	free(text);
	
	// PSEUDO-CODE
	// - Read in entire input file into char buffer
	// - Kasiski Analysis
	//   - Read word_len bytes from input buffer
	//   - If they're not all lowercase letters, skip it
	//   - If they are, iterate through all remaining bytes after current index
	//   - If you find another string of word_len bytes that matches the one found above
	//     - Print the desired output (length, index of each, difference between, string)
	//   - Repeat process until you've searched all of the strings in the input
	//   - If you found at least one match above, repeat the whole process with word_len++
	// - Index of Correlation Analysis
	//   - Iterate through the entire file, counting how many of each letter there are
	//   - Print the result counts for each letter
	//   - Calculate Index of Correlation using equation from book
	//     - Essentially take summation of each letter count multiplied by (itself-1)
	//       and divide by (L*(L-1)), where L is number of letters in file
	//   - Calculate Expected Index of Correlation for each period length up to max_t
	//     - Also from equation in book, shown above
	// - Auto-correlation Analysis
	//   - Iterate through entire ciphertext file
	//   - For each character, check if text[i] and text[i+per] are both letters
	//   - If they are, check if they match
	//   - If they're the same letter, increment correlated match count
	//   - Print output displaying number of matches for each period
}
