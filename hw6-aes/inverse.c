// Kevin Crane
//   CS531 - Homework 6
//   inverse.c

#include "inverse.h"


// Initialize 256-char table of multiplicative inverses
//	through AES' finite fielld (mod x^4 + 1)
void init_inv_table(unsigned char inv_table[256]) {
	// Hardcoded table of multiplicative inverses
	char* table_list;// = (char*)calloc(1, 256);
	table_list = (char*)"00018df6cb527bd1e84f29c0b0e1e5c7\
74b4aa4b992b605f583ffdccff40eeb2\
3a6e5af1554da8c9c10a98153044a2c2\
2c45926cf3396642f235206f77bb5919\
1dfe37672d31f569a764ab135425e909\
ed5c05ca4c2487bf183e22f051ec6117\
165eafd349a63643f44791df3393213b\
79b7978510b5ba3cb670d006a1fa8182\
837e7f809673be569b9e95d9f702b9a4\
de6a326dd88a84722a149f88f9dc899a\
fb7c2ec38fb8654826c8124acee7d262\
0ce01fef11757871a58e763dbdbc8657\
0b282fa3dad4e40fa92753041bfcace6\
7a07ae63c5dbe2ea948bc4d59df8906b\
b10dd6ebc60ecfad084ed7e35d501eb3\
5b2338346846038cdd9c7da0cd1a411c";

	// Read each pair of hexvals, store as ASCII vals in inv_table
	char temp_val[3];
	for(int i=0; i<256; i++) {
		temp_val[0] = table_list[i*2];
		temp_val[1] = table_list[i*2+1];
		temp_val[2] = ' ';
		inv_table[i] = strtol(temp_val, NULL, 16);
	}
}


// Calculate the multiplicative inverse of the polynomial with coefficients in GF(2^8)
//	Store the results in inv_poly (input: char array of 4 bytes as coefficients)
void get_inverse(unsigned char poly[4], unsigned char inv_table[256], unsigned char inv_poly[4]) {
	// Declare default arrays for a, b, remainder, quotient, aux
	unsigned char rem[6][5] = { { 0x01, 0x00, 0x00, 0x00, 0x01 }, { 0x00, poly[0], poly[1], poly[2], poly[3] } };
	unsigned char quo[6][4] = { { 0 } };
	unsigned char aux[6][4] = { { 0 }, { rem[0][1], rem[0][2], rem[0][3], rem[0][4] } };
	
	// Print first two rows of output (should be pretty much the same every time)
	printf("i=1, rem[i]={%02x}{%02x}{%02x}{%02x}, quo[i]={00}{00}{00}{00}, aux[i]={00}{00}{00}{00}\n",
			rem[0][1], rem[0][2], rem[0][3], rem[0][4]);
	printf("i=2, rem[i]={%02x}{%02x}{%02x}{%02x}, quo[i]={00}{00}{00}{00}, aux[i]={%02x}{%02x}{%02x}{%02x}\n",
			poly[0], poly[1], poly[2], poly[3],
			aux[1][0], aux[1][1], aux[1][2], aux[1][3]);
	
	// Iterate up to 4 more times, computing rem, quo, and aux; find inv_poly if applicable
	for(int i=2; i<6; i++) {
		// Super shitty code, but I've spent hours looking at how to do this and I'm tired.
		// First half of quotient
		unsigned char temp_math[6-i];
		quo[i][2] = mult(inv_table[ rem[i-1][i-1] ], rem[i-2][i-2]);
		for(int j=0; j<(6-i); j++) {
			temp_math[j] = rem[i-2][j+i-1] ^ mult( rem[i-1][j+i], quo[i][2] );
		}
		
		// Second half of quotient
		if(i<5) {
			temp_math[5-i] = rem[i-2][4];
			quo[i][3] = mult(inv_table[ rem[i-1][i-1] ], temp_math[0]);
		} else {
			// Find value of end of quotient that leads to remainder of 0x01
			quo[i][3] = mult(inv_table[ rem[i-1][i-1] ], temp_math[0]^0x01);
			rem[i][i-1] = 0x01;
		}
		
		// Compute remainder from quotient and previous remainders
		for(int j=i; j<5; j++) {
			rem[i][j] = temp_math[j-i+1] ^ mult( rem[i-1][j], quo[i][3] );
		}
		
		// Compute aux array values
		get_modprod(quo[i], aux[i-1], aux[i]);
		for(int j=0; j<4; j++) {
			aux[i][j] ^= aux[i-2][j];
		}
		
		// Output current results
		printf("i=%d, rem[i]={%02x}{%02x}{%02x}{%02x}, quo[i]={%02x}{%02x}{%02x}{%02x}, aux[i]={%02x}{%02x}{%02x}{%02x}\n",
			i+1, rem[i][1], rem[i][2], rem[i][3], rem[i][4],
			quo[i][0], quo[i][1], quo[i][2], quo[i][3],
			aux[i][0], aux[i][1], aux[i][2], aux[i][3]);
		
		// Invalid coefficient, no inverse
		if(i != 5 && rem[i][i] == 0x00) {
			printf("{%02x}{%02x}{%02x}{%02x} does not have a multiplicative inverse.\n", 
					poly[0], poly[1], poly[2], poly[3]);
			return;
		}
	}
	
	// Store final inv_poly values
	for(int i=0; i<4; i++) {
		inv_poly[i] = aux[5][i];
	}
	printf("Multiplicative inverse of {%02x}{%02x}{%02x}{%02x} is {%02x}{%02x}{%02x}{%02x}\n",
			poly[0], poly[1], poly[2], poly[3],
			inv_poly[0], inv_poly[1], inv_poly[2], inv_poly[3]);
}

// Calculate the multiplicative inverse of the polynomial with coefficients in GF(2^8)
void inverse(char* poly) {	
	unsigned char* poly_bytes = (unsigned char*)calloc(1,4);
	unsigned char* inv_poly = (unsigned char*)calloc(1,4);
	unsigned char* inv_table = (unsigned char*)calloc(1,256);
	init_inv_table(inv_table);
	
	// Error checking and converting chars to raw bytes
	if(strlen(poly) != 8) {
		fprintf(stderr, "ERROR: polynomial must consist of 8 characters, all hex values.\n");
		return;
	} else {
		char temp_val[3];
		for(int i=0; i<4; i++) {
			if(!is_hex_char(poly[i*2]) || !is_hex_char(poly[i*2+1])) {
				fprintf(stderr, "ERROR: all values in the polynomial must be hex values.\n");
				return;
			}
			temp_val[0] = poly[i*2];
			temp_val[1] = poly[i*2+1];
			temp_val[2] = ' ';
			poly_bytes[i] = strtol(temp_val, NULL, 16);
		}
	}
	
	// Compute the multiplicative inverse, store in inv_poly
	get_inverse((unsigned char*)poly_bytes, inv_table, inv_poly);
	
	free(poly_bytes);
	free(inv_poly);
	free(inv_table);
	
	
	// PSEUDO-CODE
	// - Initialize the table of inverses for bytes in the AES finite field
	// - Convert the 8 hexchars of input into 4 bytes of poly coefficients
	// - Given the input polynomial and inv_table, compute the inverse polynomial
	//   - Print the first 2 lines of output (more or less the same depending on polynomial)
	//   - Using janky, awkward math perform long division with polynomials each round
	//     - Use inv_table to determine what to mult divisor by to eliminate first term
	//     - XOR add this term and repeat with this (temp_math)
	//     - This will give quotient (2 bytes) and remainder (one fewer byte than the prior round)
	//     - Compute aux[i] for this round with: modprod(quo[i], aux[i-1]) - aux[i-2]
	//   - Repeat up to 4 times (6 total rounds max) with smaller and smaller remainders
	//   - Eventually remainder should end up as 0x00000001. Done, output aux[6] and be done
	//   * If remainder becomes 0x00000000, this polynomial has no inverse and should be ended
}
