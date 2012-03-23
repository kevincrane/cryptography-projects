// Kevin Crane
//   CS531 - Homework 5
//   encrypt.c

#include "encrypt.h"


// Quick methods for easy access to single bits
bool get_bit(unsigned char* string, int index) {
	return (( (string[index/8]) >> (7 - (index%8)) ) & 0x01);
}
void set_bit(unsigned char* string, int index) {
	string[index/8] = string[index/8] | (0x01 << (7 - ((index)%8)));
}
void clear_bit(unsigned char* string, int index) {
	string[index/8] = string[index/8] & ~(0x01 << (7 - ((index)%8)));
}

// Concatanate sub-keys C and D and print the output to stderr
void print_keys_cd(unsigned char* C, unsigned char* D, unsigned char* T, int round) {
	memcpy(T, C, 3);
	*(T+3) = *(C+3) | (*D);
	memcpy(T+4, D+1, 3);
	fprintf(stderr, "(C%d,D%d)=%02x%02x%02x%02x%02x%02x%02x\n", round, round, 				//	UNCOMMENT THESE BEFORE SUBMISSION KEVIN!
			*T, *(T+1), *(T+2), *(T+3), *(T+4), *(T+5), *(T+6));
}

// Circular shifts a string to the left by 'shift' places
void circ_shift_left(unsigned char* val, int shift, int start, int numbits) {
	for(int rounds=0; rounds<shift; rounds++) {
		bool first_val = get_bit(val, start);
		for(int i=start; i<start+numbits-1; i++) {
			if(get_bit(val, i+1)) {
				set_bit(val, i);
			} else {
				clear_bit(val, i);
			}
		}
		if(first_val) {
			set_bit(val, start+numbits-1);
		} else {
			clear_bit(val, start+numbits-1);
		}
	}
}


// ###########################################

// Returns true if ch is a hex character (0-9, a-f)
bool is_hex_char(char ch) {
	if(ch >= '0' && ch <= '9') {
		return true;
	} else if(ch >= 'a' && ch <= 'f') {
		return true;
	} else if(ch >= 'A' && ch <= 'F') {
		return true;
	} else {
		return false;
	}
}


// Generate an array of 16 48-bit keys from initial 64-bit key
bool gen_keys(char *init_key, FILE *table, unsigned char keys[16][6]) {
	
	int pc1_vals[56] = { 0 };
	int pc2_vals[48] = { 0 };
	int v_vals[16] = { 0 };
	unsigned char* key_val = (unsigned char*)calloc(1,8);		// Used to store actual hex values from key
	unsigned char *C = (unsigned char*)calloc(1,4);			// 2 halves of current round key
	unsigned char *D = (unsigned char*)calloc(1,4);			//	Big-endian style (28 values, 4 padding chars)
	
	
	// Make sure key is valid length
	if(strlen(init_key) != 16) {
		fprintf(stderr, "ERROR: invalid key length. Must be 16 hex characters.\n");
		return false;
	} else {
		for(int i=0; i<16; i++) {
			if(!is_hex_char(init_key[i])) {
				fprintf(stderr, "ERROR: invalid key. Must all be hex characters.\n");
				return false;
			}
		}
	}
	
	// Load tablefile values into arrays
	if(!read_line_vals(table, (char*)"PC1=", pc1_vals, 56) || !read_line_vals(table, (char*)"PC2=", pc2_vals, 48)
			|| !read_line_vals(table, (char*)"V=", v_vals, 16)) {
		fprintf(stderr, "ERROR: table file has invalid values.\n");
		return false;
	}
	
	// Convert key from string of hex characters into actual hex values; kinda janky
	for(int i=0; i<8; i++) {
		char temp_val[3];
		temp_val[0] = init_key[i*2];
		temp_val[1] = init_key[i*2+1];
		temp_val[2] = ' ';
		key_val[i] = strtoul(temp_val, NULL, 16);
	}
	
	// Determine initial C and D values (C[28], D[28] permute from PC1 values)
	for(int i=0; i<28; i++) {
		// Set i'th place of C value?
		if(get_bit(key_val, pc1_vals[i]-1)) {
			set_bit(C, i);
		}
		// Set i'th place of D value?
		if(get_bit(key_val, pc1_vals[i+28]-1)) {
			set_bit(D, i+4);
		}
	}
	
	// Print initial round 0 sub-key
	unsigned char* T = (unsigned char*)malloc(7);
	print_keys_cd(C, D, T, 0);
	
	// Compute the 16 round keys
	for(int i=0; i<16; i++) {
		// Circle shift to the left by a number of spaces dictated in V vals
		circ_shift_left(C, v_vals[i], 0, 28);
		circ_shift_left(D, v_vals[i], 4, 28);
		print_keys_cd(C, D, T, i+1);
		
		// Use PC2 values to permutate each the bits in this key
		for(int j=0; j<48; j++) {
			// If value of PC2[j] is set, set the j'th position of the key
			if(get_bit(T, pc2_vals[j]-1)) {
				set_bit(keys[i], j);
			}
		}
		
		// Output the key for this round
		fprintf(stderr, "k%d=%02x%02x%02x%02x%02x%02x\n", i+1, keys[i][0], keys[i][1],				//UNCOMMENT THESE BEFORE SUBMISSION KEVIN!
				keys[i][2], keys[i][3], keys[i][4], keys[i][5]);
	}
	free(T);
	
	
	// Free the memory and leave
	free(key_val);
	free(C);
	free(D);
	
	return true;
}

// Returns true if the round keys are weak (palindromic), false otherwise
bool is_weak_key(unsigned char keys[16][6]) {
	for(int i=0; i<8; i++) {
		if(strncmp((const char*)keys[i], (const char*)keys[15-i], 6) != 0) {
			return false;
		}
	}
	return true;
}


// ###################################


// Calculate and apply the initial permutation function
void initial_permutation(unsigned char* input, unsigned char* cipher_block, unsigned char* L, 
		unsigned char* R, int ip_vals[64], int ip_inv_vals[64], FILE* table, bool first) {
	
	// Load tablefile values into arrays
	if(!read_line_vals(table, (char*)"IP=", ip_vals, 64)) {
		fprintf(stderr, "ERROR: table file has invalid values.\n");
		return;
	}
	
	// Compute inverse IP table
	for(int i=0; i<64; i++) {
		for(int j=0; j<64; j++) {
			// Iterate through IP table until you find the next index
			if(ip_vals[j] == (i+1)) {
				ip_inv_vals[i] = j+1;
				break;
			}
		}
	}
	
	// Apply the Initial Permutation on the 8-byte input block
	for(int i=0; i<64; i++) {
		// If value of IP[i] is set, set the i'th position of the init ciphertext
		if(get_bit(input, ip_vals[i]-1)) {
			set_bit(cipher_block, i);
		} else {
			clear_bit(cipher_block, i);
		}
	}
	memcpy(L, cipher_block, 4);
	memcpy(R, cipher_block+4, 4);
	
	// Print current L and R values to stderr
	if(first) {
		fprintf(stderr, "(L0,R0)=%02x%02x%02x%02x%02x%02x%02x%02x\n",
				*L, *(L+1), *(L+2), *(L+3), 
				*R, *(R+1), *(R+2), *(R+3) );
	}
}

// Perform the Inverse Permutation function using the inverse table found earlier
void inverse_permutation(unsigned char* cipher_block, unsigned char* temp_block, 
		unsigned char* L, unsigned char* R, int ip_inv_vals[64]) {
	
	memcpy(temp_block, R, 4);
	memcpy(temp_block+4, L, 4);
	for(int i=0; i<64; i++) {
		// If value of IP-1[i] is set, set the i'th position of the init ciphertext
		if(get_bit(temp_block, ip_inv_vals[i]-1)) {
			set_bit(cipher_block, i);
		} else {
			clear_bit(cipher_block, i);
		}
	}
}


// Perform the Expansion function, converting 32-bit block R to 48-bits
void expand(unsigned char* R, unsigned char* ER, FILE* table) {
	int e_vals[48] = { 0 };
	
	// Load tablefile values into arrays
	if(!read_line_vals(table, (char*)"E=", e_vals, 48)) {
		fprintf(stderr, "ERROR: table file has invalid values.\n");
		return;
	}
	
	// Set appropriate bits in E(R), dictated by E vals
	for(int i=0; i<48; i++) {
		if(get_bit(R, e_vals[i]-1)) {
			set_bit(ER, i);
		} else {
			clear_bit(ER, i);
		}
	}
}

// Perform the Substitution function using 8 S-Blocks; convert 48-bit cipher back to 32-bit
void s_blocks(unsigned char* ER, unsigned char* SR, FILE* table) {
	unsigned int s_vals[64] = { 0 };
	char header[3];
	int row, col;

	// Reset S(R) values
	for(int i=0; i<4; i++)
		SR[i] = 0x00;
	
	// For each S# block
	for(int i=0; i<8; i++) {
		// Load tablefile values into arrays
		sprintf(header, "S%d=", i+1);
		if(!read_line_vals(table, header, (int*)s_vals, 64)) {
			fprintf(stderr, "ERROR: table file has invalid values.\n");
			return;
		}
		
		// Substitute 6 bit chunks from input E(R) and place in 4-bit blocks S(R)
		//	Repeat for all 8 S-Blocks
		row = (2*get_bit(ER, i*6)) + get_bit(ER, (i*6)+5);
		col = ( (8*get_bit(ER, (i*6)+1)) + (4*get_bit(ER, (i*6)+2)) +
				(2*get_bit(ER, (i*6)+3)) + (1*get_bit(ER, (i*6)+4)) );
		
		// Add values from S-Block into 32-bit S(R) block
		*(SR + i/2) |= ( s_vals[ 16*row + col ] << (4 * ((i+1)%2)) );
	}
}

// Perform the Permutation function, permuting the 32-bit block SR into final block R
void perm_block(unsigned char* SR, unsigned char* R, FILE* table) {
	int p_vals[32] = { 0 };
	
	// Load tablefile values into arrays
	if(!read_line_vals(table, (char*)"P=", p_vals, 32)) {
		fprintf(stderr, "ERROR: table file has invalid values.\n");
		return;
	}
	
	// Apply the Permutation function on the 32-bit block
	for(int i=0; i<32; i++) {
		// If value of P[i] is set, set the i'th position of the ciphertext
		if(get_bit(SR, p_vals[i]-1)) {
			set_bit(R, i);
		} else {
			clear_bit(R, i);
		}
	}
}


// Encrypt a single 8-byte block of plaintext using the DES algorithm and the 16 round keys
//	Stores result in cipher_block
void encrypt_block(unsigned char* input, unsigned char* cipher_block, unsigned char keys[16][6], FILE* table, bool first) {
	
	int ip_vals[64] = { 0 };
	int ip_inv_vals[64] = { 0 };
	unsigned char* L = (unsigned char*)calloc(1,4);
	unsigned char* R = (unsigned char*)calloc(1,4);
	unsigned char* ER = (unsigned char*)calloc(1,6);
	unsigned char* SR = (unsigned char*)calloc(1,4);
	unsigned char* old_L = (unsigned char*)calloc(1,4);
	
	// Calculate and apply the initial permutation function
	initial_permutation(input, cipher_block, L, R, ip_vals, ip_inv_vals, table, first);
			
	// Compute the 16 round ciphertext encryption
	for(int i=0; i<16; i++) {
		// Swap R(i-1) to L(i)
		memcpy(old_L, L, 4);
		memcpy(L, R, 4);
		
		// Expansion function
		expand(R, ER, table);
		
		// XOR with round key
		for(int j=0; j<6; j++) {
			ER[j] ^= keys[i][j];
		}
		
		// S-Blocks
		s_blocks(ER, SR, table);
		
		// Permutation function
		perm_block(SR, R, table);
		
		// XOR with L(i-1)
		for(int j=0; j<4; j++) {
			R[j] ^= old_L[j];
		}
		
		// Print current L and R values to stderr
		if(first) {
			fprintf(stderr, "(L%d,R%d)=%02x%02x%02x%02x%02x%02x%02x%02x\n", i+1, i+1,
					*L, *(L+1), *(L+2), *(L+3), 
					*R, *(R+1), *(R+2), *(R+3) );
		}
	}
	
	
	// Apply the Inverse Permutation on the 8-byte input block
	unsigned char* temp_block = (unsigned char*)malloc(8);
	inverse_permutation(cipher_block, temp_block, L, R, ip_inv_vals);
	free(temp_block);
	
	free(SR);
	free(ER);
	free(L);
	free(R);
	free(old_L);
}


// Encrypts the input using DES driven by tablefile in the ECB mode using key 
//	as the encryption key (8-byte long and in hexstring format)
void encrypt(char *key, FILE *table, FILE *input) {

	unsigned char keys[16][6] = { { 0 } };
	unsigned char* input_block = (unsigned char*)calloc(1,8);
	unsigned char* cipher_block = (unsigned char*)calloc(1,8);
	
	// Verify table
	if(!tablecheck(table)) {
		return;
	}
	
	// Generate 16 round keys
	if(!gen_keys(key, table, keys)) {
		fprintf(stderr, "ERROR: did not properly generate round keys.\n");
		return;
	}
	
	// Make sure the user didn't use a weak key
	if(is_weak_key(keys)) {
		fprintf(stderr, "ERROR: the key provided is weak (palindromic) and easily broken.\n");
		return;
	}
	
	// Encrypt everything from input in 8 byte-block
	bool first = true;
	int size;
	while((size = fread(input_block, 1, 8, input))) {
		if((*input_block == '\n') && (size == 1))
			continue;
			
		// Encrypt a single block
		encrypt_block(input_block, cipher_block, keys, table, first);
		
		// Print current cipher block values to output
		printf("%c%c%c%c%c%c%c%c",
				*cipher_block, *(cipher_block+1), *(cipher_block+2), *(cipher_block+3), 
				*(cipher_block+4), *(cipher_block+5), *(cipher_block+6), *(cipher_block+7) );
		
		// Reset blocks for next iteration
		first = false;
		for(int i=0; i<8; i++) {
			input_block[i] = 0x00;
			cipher_block[i] = 0x00;
		}
	}
	
	free(input_block);
	free(cipher_block);
	
	
	// PSEUDO-CODE
	// - Key Generation
	//   - The user inputs a 8 byte key as 16 hex characters
	//   - First, this key must be converted from a 16-byte string to an 8-byte string
	//     by parsing the hex characters
	//   - Next, the tablefile is read and the rows for V, PC1, and PC2 are stored in arrays
	//   - The initial C and D values are generated by removing the parity bits
	//     (every bit divisible by 8) and permutating the remaining bits against PC1 values.
	//   - For each of the 16 rounds:
	//     - Circular shift C and D values by 1 or 2 spaces, dictated by V values
	//     - Take these new values, concatanate them, and permutate based on PC2 values
	//     - Store this round's key and repeat
	//   - Should now have array with 16 48-bit keys (6-bytes each).
	// - Break original input into multiple 8-byte blocks
	// - Encryption (perform for each 8-byte block)
	//   - Perform the Initial Permutation function, split into 2 32-bit halves (L0 and R0)
	//   - Repeat the following steps for 16 rounds (i)
	//   - L(i) = R(i-1)
	//   - Expansion (performed on half R(i-1))
	//     - Using the E values in the table, expand the original 32-bit value to 48-bits
	//   - XOR this 48-bit value with 48-bit k(i)
	//   - Substitution
	//     - Using the 8 S# values in the tablefile:
	//     - Split the 48-bit block into 8 6-bit blocks
	//     - Read the values from the tablefile to convert each 6-bit block to 4-bits
	//     - This should leave you with 8 4-bit blocks (32-bit value)
	//   - Permutation
	//     - Use P values from tablefile to permute the R(i-1) values
	//   - XOR R(i-1) with L(i-1), set the result to R(i)
	//   - Repeat this 16 times, except not swapping sides on round 16
	//   - Perform the Inverse Initial Permutation on the final output
	// - Print the 8-byte output from above, repeat until input complete
}
