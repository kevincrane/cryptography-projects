// Kevin Crane
//   CS531 - Homework 6
//   decrypt.c

#include "decrypt.h"


// Prints the current contents of the state, plus a line header
void inv_print_state(unsigned char state[4][4], int round, const char* header) {
	// Print line header
	if(round < 10) {
		printf("round[ %d].%s   ", round, header);
	} else {
		if(strcmp(header, "ioutput") == 0) {
			printf("round[%d].%s  ", round, header);
		} else {
			printf("round[%d].%s   ", round, header);
		}
	}
	// Print byte values
	if(strcmp(header, "ik_sch") == 0) {
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


// Inverts AES's 256-byte S-Box array
void invert_s_box(unsigned char* s_box, unsigned char* inv_s_box) {
	// S-box contains permutation of bytes from 0x00 to 0xff
	// 	e.g. if s_box[0] contains 0x63, inv_s_box[0x63] contains 0x00
	for(int i=0; i<256; i++) {
		inv_s_box[ s_box[i] ] = i;
	}
}


// Shifts each byte in each row to the right by a number of spaces
//	2nd row shifts 1, 3rd shifts 2, 4th row shifts 3
void inv_shift_rows(unsigned char state[4][4]) {
	unsigned char temp_row[3];
	// Iterate through rows 1-3
	for(int row=1; row<4; row++) {
		// Move (row) bytes into temp array
		for(int i=3; i>=4-row; i--) {
			temp_row[i+row-4] = state[row][i];
		}
		// Rotate bytes circularly
		for(int col=3; col>=0; col--) {
			if((col-row) >= 0) {
				// Shift bytes over to right
				state[row][col] = state[row][col-row];
			} else {
				// Sub in bytes from temp row
				state[row][col] = temp_row[col];
			}
		}
	}
}




// ######################################


// Decrypts the input using AES-128 driven by tablefile; stores in output[4][4]
void decrypt_block(unsigned char input_block[16], unsigned char output_block[16],
		unsigned char round_key[44][4], FILE* table) {
	
	unsigned char state[4][4] = { { 0 } };	// Carries state of block through encryption
	unsigned char* inv_poly = (unsigned char*)calloc(1,4);
	
	// Initialize Substitution Boxes
	unsigned char* s_box = (unsigned char*)calloc(1,256);
	unsigned char* inv_s_box = (unsigned char*)calloc(1,256);
	init_s_box(s_box, table);
	invert_s_box(s_box, inv_s_box);
	free(s_box);
	init_poly(inv_poly, table, "INVP=");
	
	// Copy input to initital state
	for(int i=0; i<16; i++) {
		state[i%4][i/4] = input_block[i];
	}
	
	// Print initial inputs and key schedule
	inv_print_state(state, 0, "iinput");
	inv_print_state(&(round_key[4*10]), 0, "ik_sch");
	
	// Add initial round key
	add_round_key(state, round_key, 10);
	
	// Perform operations for rounds 1 to 9
	for(int i=9; i>0; i--) {
		inv_print_state(state, 10-i, "istart");
		
		// Inverse Shift Rows
		inv_shift_rows(state);
		inv_print_state(state, 10-i, "is_row");
		
		// Substitute Bytes
		sub_bytes(state, inv_s_box);
		inv_print_state(state, 10-i, "is_box");
		
		// Add Round Key
		inv_print_state(&(round_key[4*i]), 10-i, "ik_sch");
		add_round_key(state, round_key, i);
		inv_print_state(state, 10-i, "ik_add");
		
		// Mix Columns
		mix_columns(state, inv_poly);
	}
	
	// Final round operations
	inv_print_state(state, 10, "istart");
	
	// Shift Rows
	inv_shift_rows(state);
	inv_print_state(state, 10, "is_row");
	
	// Substitute Bytes
	sub_bytes(state, inv_s_box);
	inv_print_state(state, 10, "is_box");
	
	// Add Round Key
	add_round_key(state, round_key, 0);
	inv_print_state(&(round_key[4*0]), 10, "ik_sch");
	
	// Store final state value in output_block, done
	for(int i=0; i<16; i++) {
		output_block[i] = state[i%4][i/4];
	}
	inv_print_state(state, 10, "ioutput");
	
	free(inv_poly);
	free(inv_s_box);
}


// #####################################


// Decrypts the input using AES-128 driven by tablefile in the ECB mode using key 
//	as the decryption key (16-byte long and in hexstring format)
void decrypt(char *key_chars, FILE *table, FILE *input) {

	unsigned char round_key[44][4] = { { 0 } };
	unsigned char* init_key = (unsigned char*)calloc(1,16);
	unsigned char* cipher_block = (unsigned char*)calloc(1,16);
	unsigned char* plain_block = (unsigned char*)calloc(1,16);

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
	if(fread(cipher_block, 1, 16, input) < 16) {
		fprintf(stderr, "ERROR: input file was less than 16 bytes long.\n");
		return;
	}
	
	// Perform AES decryption on a single block
	//	Easy to modify to encrypt whole document
	//		- Send print statements to stderr in encrypt_block, print output_block to stdout
	//		- Read 16 bytes, encrypt, repeat, until nothing left to read
	decrypt_block(cipher_block, plain_block, round_key, table);
	
	
	free(init_key);
	free(plain_block);
	free(cipher_block);


	// PSEUDO-CODE
	// - Same as AES-128 encrypt, except for a few minor changes:
	//   - The S-box needs to be inverted before it can be used for substitution
	//   - You use the inverse polynomial instead of the original for mix_columns
	//   - During shift_rows, you shift each row to the right instead of the left
	//   - The round keys are used in reverse order
	// - The order of core operations is also slightly different
	//   - Inv Shift Columns
	//   - Substitute Bytes (inverse S-box)
	//   - Add Round Key
	//   - Mix Columns (inverse polynomial)
}
