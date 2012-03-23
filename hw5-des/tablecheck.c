// Kevin Crane
//   CS531 - Homework 5
//   tablecheck.c

#include "tablecheck.h"


// Search the input file for the row that starts with the header string, stores vals in array
bool read_line_vals(FILE *fin, char* header, int* vals, int expected_len) {

	char* line_buf = (char*)calloc(1,512);
	char* comma_ptr = NULL;
	
	// Find and read the correct line
	fseek(fin, 0, SEEK_SET);
	while(strncmp(line_buf, header, strlen(header)) != 0) {
		if(!fgets(line_buf, 512, fin)) {
			fprintf(stderr, "ERROR: line for '%s' not found in tablefile.\n", header);
			return false;
		}
	}
	
	// Read through ints in line, stopping at each comma delimiter
	comma_ptr = line_buf + strlen(header) - 1;
	for(int i=0; i<expected_len; i++) {
		// If it reaches an early end, not enough values in line
		if(comma_ptr == NULL) {
			fprintf(stderr, "ERROR: insufficient values for line '%s' in tablefile.\n", header);
			return false;
		}
	
		// Read integer, add it to vals array, move pointer to just after comma
		vals[i] = atoi(comma_ptr + 1);
		comma_ptr = strchr(comma_ptr+1, ',');
	}
	
	// Make sure there are no extra values in the string
	if(comma_ptr != NULL) {
		fprintf(stderr, "ERROR: too many values in line '%s'.\n", header);
		return false;
	}
	
	free(line_buf);
	return true;
}

// ########################################


// Check given int array to determine if IP (Initial Permutation) values are valid
//	Permutation of values 1 to 64 inclusive, must appear once each
bool check_ip(FILE* table) {
	char* header = (char*)"IP=";
	int val_count[64];
	int ip_vals[64];
	
	// Initialize val_count to all 0's
	for(int i=0; i<64; i++)
		val_count[i] = 0;
		
	// Read all int values for IP into array
	if(!read_line_vals(table, header, ip_vals, 64)) {
		// Error message printed in read_line_vals
		return false;
	}
	
	// Iterate through all read values
	for(int i=0; i<64; i++) {
		// Make sure IP value is between 1 and 64, inclusive
		if(ip_vals[i] < 1 || ip_vals[i] > 64) {
			fprintf(stderr, "ERROR: IP value is of invalid size %d.\n", ip_vals[i]);
			return false;
		}
		
		// Increment count of current IP value
		val_count[ ip_vals[i]-1 ]++;
	}
	
	// Now make sure that all values 1 to 64 are accounted for
	for(int i=0; i<64; i++) {
		if(val_count[i] != 1) {
			fprintf(stderr, "ERROR: not every IP value (%d) was found exactly once.\n", i+1);
			return false;
		}
	}
	
	// Everything checks out
	return true;
}

// Check given int array to determine if E (Expansion) values are valid
//	48 values, 1 to 32 inclusive; must include once or twice each
bool check_e(FILE* table) {
	char* header = (char*)"E=";
	int val_count[32];
	int e_vals[48];
	
	// Initialize val_count to all 0's
	for(int i=0; i<32; i++)
		val_count[i] = 0;
		
	// Read all int values for E into array
	if(!read_line_vals(table, header, e_vals, 48)) {
		// Error message printed in read_line_vals
		return false;
	}
	
	// Iterate through all read values
	for(int i=0; i<48; i++) {
		// Make sure IP value is between 1 and 32, inclusive
		if(e_vals[i] < 1 || e_vals[i] > 32) {
			fprintf(stderr, "ERROR: E value is of invalid size %d.\n", e_vals[i]);
			return false;
		}
		
		// Increment count of current E value
		val_count[ e_vals[i]-1 ]++;
	}
	
	// Now make sure that all values 1 to 32 are accounted for
	for(int i=0; i<32; i++) {
		if(!(val_count[i] == 1 || val_count[i] == 2)) {
			fprintf(stderr, "ERROR: not every E value (%d) was found exactly once or twice.\n", i+1);
			return false;
		}
	}
	
	// Everything checks out
	return true;
}

// Check given int array to determine if P (Permutation) values are valid
//	Permutation of values 1 to 32 inclusive, must appear once each
bool check_p(FILE* table) {
	char* header = (char*)"P=";
	int val_count[32];
	int p_vals[32];
	
	// Initialize val_count to all 0's
	for(int i=0; i<32; i++)
		val_count[i] = 0;
		
	// Read all int values for P into array
	if(!read_line_vals(table, header, p_vals, 32)) {
		// Error message printed in read_line_vals
		return false;
	}
	
	// Iterate through all read values
	for(int i=0; i<32; i++) {
		// Make sure P value is between 1 and 32, inclusive
		if(p_vals[i] < 1 || p_vals[i] > 32) {
			fprintf(stderr, "ERROR: P value is of invalid size %d.\n", p_vals[i]);
			return false;
		}
		
		// Increment count of current P value
		val_count[ p_vals[i]-1 ]++;
	}
	
	// Now make sure that all values 1 to 32 are accounted for
	for(int i=0; i<32; i++) {
		if(val_count[i] != 1) {
			fprintf(stderr, "ERROR: not every P value (%d) was found exactly once.\n", i+1);
			return false;
		}
	}
	
	// Everything checks out
	return true;
}

// Check given int array to determine if S# (Substitution) values are valid
//	64 values in 4 groups, permutation of 0 to 15 inclusive in each group
bool check_s(FILE* table, char* header) {
	int val_count[16];
	int s_vals[64];
	
	// Initialize val_count to all 0's
	for(int i=0; i<16; i++)
		val_count[i] = 0;
		
	// Read all int values for S into array
	if(!read_line_vals(table, header, s_vals, 64)) {
		// Error message printed in read_line_vals
		return false;
	}
	
	// Iterate through each of 4 groups, 16 values each
	for(int i=0; i<4; i++) {
		for(int j=0; j<16; j++) {
			// Make sure S value for current group is between 0 and 15, inclusive
			if(s_vals[i*16 + j] < 0 || s_vals[i*16 + j] > 15) {
				fprintf(stderr, "ERROR: S value in group %d is of invalid size %d.\n", i, s_vals[i*16 + j]);
				return false;
			}
		
			// Increment count of current P value
			val_count[ s_vals[i*16 + j] ]++;
		}
		
		// Now make sure that all values 0 to 15 are accounted for
		for(int j=0; j<16; j++) {
			if(val_count[j] != 1) {
				fprintf(stderr, "ERROR: not every S value (%d) in group %d was found exactly once.\n", j, i);
				return false;
			}
			val_count[j] = 0;
		}
	}
	
	// Everything checks out
	return true;
}

// Check given int array to determine if V (Circular Rotation) values are valid
//	16 values; should have 12 2's and 4 1's
bool check_v(FILE* table) {
	char* header = (char*)"V=";
	int val_count[2];
	int v_vals[16];
	
	// Initialize val_count to all 0's
	for(int i=0; i<2; i++)
		val_count[i] = 0;
		
	// Read all int values for V into array
	if(!read_line_vals(table, header, v_vals, 16)) {
		// Error message printed in read_line_vals
		return false;
	}
	
	// Iterate through all read values
	for(int i=0; i<16; i++) {
		// Make sure V value is either 1 or 2
		if(!(v_vals[i] == 1 || v_vals[i] == 2)) {
			fprintf(stderr, "ERROR: V value is of invalid size %d.\n", v_vals[i]);
			return false;
		}
		
		// Increment count of current P value
		val_count[ v_vals[i]-1 ]++;
	}
	
	// Now make sure that all 1's and 2's accounted for
	if(!(val_count[0] == 4 && val_count[1] == 12)) {
		fprintf(stderr, "ERROR: invalid number of 1's and 2's found in V values.\n");
		return false;
	}
	
	// Everything checks out
	return true;
}

// Check given int array to determine if PC1 (Permuted Choice 1) values are valid
//	Permutation of values 1 to 64 inclusive, multiples of 8 removed
bool check_pc1(FILE* table) {
	char* header = (char*)"PC1=";
	int val_count[64];
	int pc_vals[56];
	
	// Initialize val_count to all 0's
	for(int i=0; i<64; i++)
		val_count[i] = 0;
		
	// Read all int values for PC1 into array
	if(!read_line_vals(table, header, pc_vals, 56)) {
		// Error message printed in read_line_vals
		return false;
	}
	
	// Iterate through all read values
	for(int i=0; i<56; i++) {
		// Make sure PC1 value is between 1 and 64, inclusive
		if(pc_vals[i] < 1 || pc_vals[i] > 64) {
			fprintf(stderr, "ERROR: PC1 value is of invalid size %d.\n", pc_vals[i]);
			return false;
		}
		
		// Increment count of current PC1 value
		val_count[ pc_vals[i]-1 ]++;
	}
	
	// Now make sure that all values 1 to 64 are accounted for, except for mults of 8
	for(int i=0; i<64; i++) {
		if((i+1)%8 == 0 && val_count[i] != 0) {
			fprintf(stderr, "ERROR: PC1 values should not contain multiples of 8 (e.g. %d).\n", i+1);
			return false;
		}
		else if((i+1)%8 != 0 && val_count[i] != 1) {
			fprintf(stderr, "ERROR: PC1 value (%d) not found exactly once.\n", i+1);
			return false;
		}
	}
	
	// Everything checks out
	return true;
}

// Check given int array to determine if PC2 (Permuted Choice 2) values are valid
//	Permutation of values 1 to 56 inclusive, 8 random values removed
bool check_pc2(FILE* table) {
	char* header = (char*)"PC2=";
	int val_count[56];
	int pc_vals[48];
	
	// Initialize val_count to all 0's
	for(int i=0; i<56; i++)
		val_count[i] = 0;
		
	// Read all int values for PC2 into array
	if(!read_line_vals(table, header, pc_vals, 48)) {
		// Error message printed in read_line_vals
		return false;
	}
	
	// Iterate through all read values
	for(int i=0; i<48; i++) {
		// Make sure PC2 value is between 1 and 64, inclusive
		if(pc_vals[i] < 1 || pc_vals[i] > 56) {
			fprintf(stderr, "ERROR: PC2 value is of invalid size %d.\n", pc_vals[i]);
			return false;
		}
		
		// Increment count of current PC2 value
		val_count[ pc_vals[i]-1 ]++;
	}
	
	// Now make sure that all values 1 to 56 are accounted for, except for 8 random ones
	for(int i=0; i<56; i++) {
		if(!(val_count[i] == 0 || val_count[i] == 1)) {
			fprintf(stderr, "ERROR: PC2 value (%d) not found exactly 0 or 1 times.\n", i+1);
			return false;
		}
	}
	
	// Everything checks out
	return true;
}


// Check the integrity of tablefile
bool tablecheck(FILE *table) {

	// Check IP vals
	if(!check_ip(table)) {
		// IP vals are invalid
		fprintf(stderr, "ERROR: tablecheck failed for IP (Initial Permutation) values.\n");
		return false;
	}

	// Check E vals
	if(!check_e(table)) {
		// E vals are invalid
		fprintf(stderr, "ERROR: tablecheck failed for E (Expansion) values.\n");
		return false;
	}

	// Check P vals
	if(!check_p(table)) {
		// P vals are invalid
		fprintf(stderr, "ERROR: tablecheck failed for P (Permutation) values.\n");
		return false;
	}
	
	// Check S# vals
	char header[3];
	for(int i=1; i<=8; i++) {
		sprintf(header, "S%d=", i);
		if(!check_s(table, header)) {
			// S# vals are invalid
			fprintf(stderr, "ERROR: tablecheck failed for S%d (Substitution) values.\n", i);
			return false;
		}
	}
	
	// Check V vals
	if(!check_v(table)) {
		// V vals are invalid
		fprintf(stderr, "ERROR: tablecheck failed for V (Circular Rotation) values.\n");
		return false;
	}
	
	// Check PC1 vals
	if(!check_pc1(table)) {
		// PC1 vals are invalid
		fprintf(stderr, "ERROR: tablecheck failed for PC1 (Permutated Choice 1) values.\n");
		return false;
	}
	
	// Check PC2 vals
	if(!check_pc2(table)) {
		// PC2 vals are invalid
		fprintf(stderr, "ERROR: tablecheck failed for PC2 (Permutated Choice 2) values.\n");
		return false;
	}
	
	return true;
	
	// PSEUDO-CODE
	// - Use read_line_vals method to search through file until finds a line starting with header
	// - Read each item in the line into an array of ints, using comma delimiters to separate
	// - Run whatever test is needed on the array to ensure that all the values are valid
	//   - Usually something like counting the instances of each number to ensure correct permutation
	// - Repeat for all headers
}
