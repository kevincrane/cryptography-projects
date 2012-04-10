// Kevin Crane
//   CS531 - Homework 6
//   keyexpand.c

#include "keyexpand.h"


#define Nk 4		// 4 words in key (4*4 bytes); standard for AES-128
#define Nr 10		// 10 rounds of encryption; standard for AES-128
#define Nb 4		// Number of bytes in a row; standard for AES


// Initialize S-Box variable and store in byte array s_box
void init_s_box(unsigned char* s_box, FILE* table) {
	const char* header = "S=";
	char* line_buf = (char*)calloc(1,640);
	
	// Find and read the correct line
	fseek(table, 0, SEEK_SET);
	while(strncmp(line_buf, header, strlen(header)) != 0) {
		if(!fgets(line_buf, 640, table)) {
			fprintf(stderr, "ERROR: line for '%s' not found in tablefile.\n", header);
			return;
		}
	}
	line_buf += strlen(header);
	
	// Iterate through each two-hexchar byte of the tablefile, convert to byte, add to next index of s_box
	char temp_val[3];
	for(unsigned int i=0; i<256; i++) {
		temp_val[0] = line_buf[i*2];
		temp_val[1] = line_buf[i*2+1];
		temp_val[2] = ' ';
		s_box[i] = strtol(temp_val, NULL, 16);
	}
}

// Initialize polynomial from tablefile
void init_poly(unsigned char* poly, FILE* table, const char* header) {
	char* line_buf = (char*)calloc(1,32);
	
	// Find and read the correct line
	fseek(table, 0, SEEK_SET);
	while(strncmp(line_buf, header, strlen(header)) != 0) {
		if(!fgets(line_buf, 32, table)) {
			fprintf(stderr, "ERROR: line for '%s' not found in tablefile.\n", header);
			return;
		}
	}
	line_buf += strlen(header);
	
	// Iterate through each two-hexchar byte of the tablefile, convert to byte, add to next index of s_box
	char temp_val[3];
	for(unsigned int i=0; i<4; i++) {
		temp_val[0] = line_buf[i*2];
		temp_val[1] = line_buf[i*2+1];
		temp_val[2] = ' ';
		poly[i] = strtol(temp_val, NULL, 16);
	}
}

// Initialize round Rijndael constant and store in array rcon
void init_rcon(unsigned char rcon[256]) {
	rcon[0] = 0x8d;
	for(int i=1; i<256; i++) {
		rcon[i] = mult(rcon[i-1], 0x02);
	}
}

// Rotates a 4-byte array one index to the left
void rot_word(unsigned char round_key[4]) {
	unsigned char temp_char = round_key[0];
	round_key[0] = round_key[1];
	round_key[1] = round_key[2];
	round_key[2] = round_key[3];
	round_key[3] = temp_char;
}


// Perform the key expansion for AES-128 using 16-byte key; each round stored in round_key
//	Pseudocode taken from Fig.11 in FIPS-197 (page 20)
void key_expansion(unsigned char init_key[16], unsigned char round_key[44][4], FILE* table) {
	unsigned char* temp = (unsigned char*)calloc(1,4);
	unsigned char* s_box = (unsigned char*)calloc(1,256);
	unsigned char* rcon = (unsigned char*)calloc(1,256);
	init_s_box(s_box, table);
	init_rcon(rcon);
	
	// Store initial round keys (form words from each piece of init key)
	for(int i=0; i<4; i++) {
		round_key[i][0] = init_key[4*i];
		round_key[i][1] = init_key[4*i+1];
		round_key[i][2] = init_key[4*i+2];
		round_key[i][3] = init_key[4*i+3];
		// Tried defining round key as a WORD (unsigned int), but I forgot there are
		//	little-endian/big-endian issues with different architectures. Fuck that.
	}
	
	// Generate subsequent round keys (from Nk -> Nb*(Nr+1))
	for(int i=4; i<44; i++) {
		// Set temp var to previous round key
		temp[0] = round_key[i-1][0];
		temp[1] = round_key[i-1][1];
		temp[2] = round_key[i-1][2];
		temp[3] = round_key[i-1][3];
		
		// Perform Rijndael key schedule core every 4 iterations
		if (i%4 == 0) {
			// For AES-128 and AES-192; rotate temp vals, put through s_box, xor the MSB with rcon
			rot_word(temp);
			temp[0] = s_box[ temp[0] ] ^ rcon[i/4];
			temp[1] = s_box[ temp[1] ];
			temp[2] = s_box[ temp[2] ];
			temp[3] = s_box[ temp[3] ];
		}
		
		// Store current round key
		round_key[i][0] = round_key[i-4][0] ^ temp[0];
		round_key[i][1] = round_key[i-4][1] ^ temp[1];
		round_key[i][2] = round_key[i-4][2] ^ temp[2];
		round_key[i][3] = round_key[i-4][3] ^ temp[3];
	}
	
	free(temp);
	free(s_box);
	free(rcon);
}

// Perform the key expansion for AES-128 using key as 16-byte string in hexstring format
void keyexpand(char* key_chars, FILE* table) {

	unsigned char* init_key = (unsigned char*)calloc(1,16);
	unsigned char round_key[Nb*(Nr+1)][4];
	
	// Verify S values are valid
	if(!check_s(table)) {
		// S vals are invalid
		fprintf(stderr, "ERROR: tablecheck failed for S (Substitution) values.\n");
		return;
	}
	// Check key length
	else if(strlen(key_chars) != 32) {
		fprintf(stderr, "ERROR: key must consist of 32 characters, all hex values.\n");
		return;
	} 
	// Read raw hexchars in and convert to bytes
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
	
	// Perform the key expansion
	key_expansion(init_key, round_key, table);
	
	// Print round output
	for(int i=0; i<44; i++) {
		if(i >= 10) {
			printf("w[%d]: %02x%02x%02x%02x\n", i, round_key[i][0], 
					round_key[i][1], round_key[i][2], round_key[i][3]);
		} else {
			printf("w[ %d]: %02x%02x%02x%02x\n", i, round_key[i][0], 
					round_key[i][1], round_key[i][2], round_key[i][3]);
		}
	}
	
	free(init_key);
	
	
	// PSEUDO-CODE
	// - Initialize the S-box and Rcon
	//   - For the S-box, read in the tablefile; incrementally assign the permutations to a 256-byte array
	//   - For Rcon, starting with rcon[0]=0x8d, modularly mult rcon[i-1] by 0x02 until you have 256 vals
	// - (For AES-128: Nk=4, Nr=10, key is 16 bytes long)
	// - For the first 4 rounds, the round_key is one quarter of the init_key
	// - For the remainder of the rounds (4*(Nr+1) total):
	//   - Every 4th round:
	//     - Rotate the round_key circularly to the left and store in temp val
	//     - Put the temp val through the s_box and XOR the MSB with the next val of Rcon
	//   - For every round, XOR the current temp vals with the previous round_key, store as next round_key
	// - Print the output of each round_key
}
