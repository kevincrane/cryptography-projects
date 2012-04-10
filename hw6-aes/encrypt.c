// Kevin Crane
//   CS531 - Homework 6
//   encrypt.c

#include "encrypt.h"


// Prints the current contents of the state, plus a line header
void print_state(unsigned char state[4][4], int round, const char* header) {
	// Print line header
	if(round < 10) {
		printf("round[ %d].%s    ", round, header);
	} else {
		if(strcmp(header, "output") == 0) {
			printf("round[%d].%s   ", round, header);
		} else {
			printf("round[%d].%s    ", round, header);
		}
	}
	// Print byte values
	if(strcmp(header, "k_sch") == 0) {
		// Print left to right, top to bottom for keys
		for(int i=0; i<16; i++) {
			printf("%02x", state[i/4][i%4]);
		}
	} else {
		// Print top to bottom, left to right for states
		for(int i=0; i<16; i++) {
			printf("%02x", state[i%4][i/4]);
		}
	}
	printf("\n");
}


// Adds the current round key to state (4 rows from round_key, 4 cols in each row)
void add_round_key(unsigned char state[4][4], unsigned char round_key[44][4], int round) {
	// Cycle through each of the 16 bytes in state
	for(int i=0; i<4; i++) {
		for(int j=0; j<4; j++) {
			// XOR current byte of round_key with byte of state
			state[i][j] ^= round_key[round*4 + j][i];
		}
	}
}

// Substitutes each byte in state according to an S-Box
void sub_bytes(unsigned char state[4][4], unsigned char* s_box) {
	// Cycle through each of the 16 bytes in state
	for(int i=0; i<4; i++) {
		for(int j=0; j<4; j++) {
			// Substitute the byte in state with one from s_box
			state[i][j] = s_box[ state[i][j] ];
		}
	}
}

// Shifts each byte in each row to the left by a number of spaces
//	2nd row shifts 1, 3rd shifts 2, 4th row shifts 3
void shift_rows(unsigned char state[4][4]) {
	unsigned char temp_row[3];
	// Iterate through rows 1-3
	for(int row=1; row<4; row++) {
		// Move (row) bytes into temp array
		for(int i=0; i<row; i++) {
			temp_row[i] = state[row][i];
		}
		// Rotate bytes circularly
		for(int col=0; col<4; col++) {
			if((col+row) < 4) {
				// Shift bytes over to left
				state[row][col] = state[row][col+row];
			} else {
				// Sub in bytes from temp row
				state[row][col] = temp_row[row+col-4];
			}
		}
	}
}

// Mix the columns of state by matrix multiplying against a given polynomial
void mix_columns(unsigned char state[4][4], unsigned char* poly) {
	unsigned char* temp_col = (unsigned char*)calloc(1,4);

	// Iterate through each column
	for(int i=0; i<4; i++) {
		memset(temp_col, 0, 4);
		// Iterate through each row of the column once
		for(int j=0; j<4; j++) {
			// Iterate through each row of column for mult/xor purposes
			for(int k=0; k<4; k++) {
				temp_col[j] ^= mult(state[k][i], poly[ (k-j+3)%4 ]);
			}
		}
		// Store updated state values in i'th col
		for(int j=0; j<4; j++) {
			state[j][i] = temp_col[j];
		}
	}
	
//	free(temp_col);
}


// ######################################


// Encrypts the input using AES-128 driven by tablefile; stores in output[4][4]
void encrypt_block(unsigned char input_block[16], unsigned char output_block[16],
		unsigned char round_key[44][4], FILE* table) {
	
	unsigned char state[4][4] = { { 0 } };	// Carries state of block through encryption
	unsigned char* poly = (unsigned char*)calloc(1,4);
	
	// Initialize Substitution Boxes
	unsigned char* s_box = (unsigned char*)calloc(1,256);
	init_s_box(s_box, table);
	init_poly(poly, table, "P=");
	
	// Copy input to initital state
	for(int i=0; i<16; i++) {
		state[i%4][i/4] = input_block[i];
	}
	
	// Print initial inputs and key schedule
	print_state(state, 0, "input");
	print_state(round_key, 0, "k_sch");
	
	// Add initial round key
	add_round_key(state, round_key, 0);
	
	// Perform operations for rounds 1 to 9
	for(int i=1; i<10; i++) {
		print_state(state, i, "start");
		
		// Substitute Bytes
		sub_bytes(state, s_box);
		print_state(state, i, "s_box");
		
		// Shift Rows
		shift_rows(state);
		print_state(state, i, "s_row");
		
		// Mix Columns
		mix_columns(state, poly);
		print_state(state, i, "m_col");
		
		// Add Round Key
		add_round_key(state, round_key, i);
		print_state(&(round_key[4*i]), i, "k_sch");
	}
	
	// Final round operations
	print_state(state, 10, "start");
	
	// Substitute Bytes
	sub_bytes(state, s_box);
	print_state(state, 10, "s_box");
	
	// Shift Rows
	shift_rows(state);
	print_state(state, 10, "s_row");
	
	// Add Round Key
	add_round_key(state, round_key, 10);
	print_state(&(round_key[4*10]), 10, "k_sch");
	
	// Store final state value in output_block, done
	for(int i=0; i<16; i++) {
		output_block[i] = state[i%4][i/4];
	}
	print_state(state, 10, "output");
	
	free(poly);
	free(s_box);
	
}


// #####################################


// Encrypts the input using AES-128 driven by tablefile in the ECB mode using key 
//	as the encryption key (16-byte long and in hexstring format)
void encrypt(char *key_chars, FILE *table, FILE *input) {

	unsigned char round_key[44][4] = { { 0 } };
	unsigned char* init_key = (unsigned char*)calloc(1,16);
	unsigned char* input_block = (unsigned char*)calloc(1,16);
	unsigned char* cipher_block = (unsigned char*)calloc(1,16);

	// Verify table
	if(!tablecheck(table)) {
		return;
	}
	
	// Check key length
	if(strlen(key_chars) != 32) {
		fprintf(stderr, "ERROR: key must consist of 32 characters, all hex values.\n");
		return;
	} 
	// Read raw key hexchars in and convert to bytes
	else {
		char temp_val[3];
		for(int i=0; i<16; i++) {
			if(!is_hex_char(key_chars[i*2]) || !is_hex_char(key_chars[i*2+1])) {
				fprintf(stderr, "ERROR: all values in the polynomial must be hex values.\n");
				return;
			}
			temp_val[0] = key_chars[i*2];
			temp_val[1] = key_chars[i*2+1];
			temp_val[2] = ' ';
			init_key[i] = strtol(temp_val, NULL, 16);
		}
	}
	
	// Perform key expansion and store result in round_key
	key_expansion(init_key, round_key, table);
	
	// Read first 16 bytes from input text
	if(fread(input_block, 1, 16, input) < 16) {
		fprintf(stderr, "ERROR: input file was less than 16 bytes long.\n");
		return;
	}
	
	// Perform AES encryption on a single block
	//	Easy to modify to encrypt whole document
	//		- Send print statements to stderr in encrypt_block, print output_block to stdout
	//		- Read 16 bytes, encrypt, repeat, until nothing left to read
	encrypt_block(input_block, cipher_block, round_key, table);
	
	
	free(init_key);
	free(input_block);
	free(cipher_block);


	// PSEUDO-CODE
	// - Break the input into 16-byte blocks (for this assignment, only one block encrypted)
	// - Store this block into a 4x4 byte array called the state (arrangement dictated in FIPS-197)
	// - Initialized S-box and perform key expansion
	// - Do an initial add_round_key on the input
	// - For 9 rounds, perform the following operations:
	//   - Substitute Bytes - Substitute based on permutation of bytes in S-box
	//   - Shift Rows - Circularly rotate the bytes in each row to the left by a certain number of spaces
	//   - Mix Columns - Multiply the values in columns by the coefficients of a 
	//       polynomial, XOR each result to get new col values
	//   - Add Round Key - XOR the next set of round keys onto the state matrix
	// - For the 10th round, repeat this process, but skip Mix Columns
	// - Store the finanl result of the state in the output_block array
	// - Print this somewhere, clear it, and repeat if you wanted to go for longer than 16-bytes of input
}
