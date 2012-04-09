// Kevin Crane
//   CS531 - Homework 6
//   modprod.c

#include <math.h>
#include "modprod.h"
#include "tablecheck.h"


/*
 * Begin code I did not write.
 * Returns xtime value of an integer x
 * The following code was obtained from: http://www.koders.com/c/fid8132B6BE3179A9AC8E48424AA876967924DF90FB.aspx?s=crc
 */
#define xtime(x) ((((x) << 1) ^ (((x) & 0x80) != 0 ? 0x1B : 0)) & 0xFF)
/*
 * End code I did not write.
 */


// Returns the ceiling of log2(n) as an integer (2^x = n; return ceiling of x)
int log2(int n) {
	if(n == 0) {
		return 0;
	}
	return (int)(log(n) / log(2));
}

// Returns the char result of coef1 x coef2
unsigned char mult(unsigned char coef1, unsigned char coef2) {
	int num_steps = log2(coef2) + 1;
	unsigned char steps[num_steps];
	
	int curr_count = coef2;
	unsigned char curr_out = 0x00;
	
	// Store incremental xtime values of the previous step
	steps[0] = coef1;
	for(int i=1; i<num_steps; i++) {
		steps[i] = xtime(steps[i-1]);
	}
	
	// XOR values incrementally until coef2 has been summed
	//	Starting from top, if 2^i < curr_count, XOR the xtime val from that step
	for(int i=num_steps-1; i>=0; i--) {
		if(pow(2, i) <= curr_count) {
			curr_out ^= steps[i];
			curr_count -= pow(2, i);
		}
	}
	return curr_out;
}

// Stores a char* to an 8-char array, containing hexchars as 4 byte coefficients in out_coef
void get_modprod(unsigned char* p1, unsigned char* p2, unsigned char out_coef[4]) {
	
	// Calculate the final coefficients of the polynomial (mod x^4 + 1)
	//	Hardcoded based on Eq. 4.12 on page 13 of FIPS-197
	out_coef[3] = mult(p1[3],p2[3]) ^ mult(p1[0],p2[2]) ^ mult(p1[1],p2[1]) ^ mult(p1[2],p2[0]);
	out_coef[2] = mult(p1[2],p2[3]) ^ mult(p1[3],p2[2]) ^ mult(p1[0],p2[1]) ^ mult(p1[1],p2[0]);
	out_coef[1] = mult(p1[1],p2[3]) ^ mult(p1[2],p2[2]) ^ mult(p1[3],p2[1]) ^ mult(p1[0],p2[0]);
	out_coef[0] = mult(p1[0],p2[3]) ^ mult(p1[1],p2[2]) ^ mult(p1[2],p2[1]) ^ mult(p1[3],p2[0]);
}

// Perform modular product of two polynomials in GF(2^8)
void modprod(char* poly1, char* poly2) {
	unsigned char* product = (unsigned char*)malloc(4);
	unsigned char p1[4];
	unsigned char p2[4];
	
	// Verify that all chars are hexchars
	if(strlen(poly1) != 8) {
		fprintf(stderr, "ERROR: poly1 must be 8 hexchars long.\n");
		return;
	} else if(strlen(poly2) != 8) {
		fprintf(stderr, "ERROR: poly2 must be 8 hexchars long.\n");
		return;
	}
	for(unsigned int i=0; i<strlen(poly1); i++) {
		if(!is_hex_char(poly2[i])) {
			fprintf(stderr, "ERROR: invalid val found for poly1. Must all be hex vals.\n");
			return;
		}
	}
	for(unsigned int i=0; i<strlen(poly2); i++) {
		if(!is_hex_char(poly2[i])) {
			fprintf(stderr, "ERROR: invalid val found for poly1. Must all be hex vals.\n");
			return;
		}
	}
	
	// Split poly1 and poly2 into 4 chars each
	char temp_val[3];
	for(int i=0; i<4; i++) {
		temp_val[0] = poly1[i*2];
		temp_val[1] = poly1[i*2+1];
		temp_val[2] = ' ';
		p1[i] = strtol(temp_val, NULL, 16);
		temp_val[0] = poly2[i*2];
		temp_val[1] = poly2[i*2+1];
		temp_val[2] = ' ';
		p2[i] = strtol(temp_val, NULL, 16);
	}
	
	// Compute modular product
	get_modprod(p1, p2, product);
	
	printf("{%c%c}{%c%c}{%c%c}{%c%c} CIRCLEX ", poly1[0], poly1[1], poly1[2], poly1[3], poly1[4], poly1[5], poly1[6], poly1[7]);
	printf("{%c%c}{%c%c}{%c%c}{%c%c} = ", poly2[0], poly2[1], poly2[2], poly2[3], poly2[4], poly2[5], poly2[6], poly2[7]);
	printf("{%02x}{%02x}{%02x}{%02x}\n", product[0], product[1], product[2], product[3]);
	free(product);
	
	
	// PSEUDO-CODE
	// - Read each of the hexchars into the byte values for 2 char arrays
	// - Given the coefficients in each of the polys
	//   - Mult and XOR each coefficient in a particular hard-coded order to 
	//     get the new coefficients (indicated in FIPS-197, eq. 4.12)
	//   - XOR
	//     - Easy, already defined by C language
	//   - Mult
	//     - In steps[0], store coef1
	//     - In steps[i], store xtime(steps[i-1]), for steps [1, log2(coef2)+1]
	//     - Each index in steps corresponds to 2^i
	//     - Starting from i=log2(coef2), step backwards to 0
	//       - Build sum equal to coef2 using values of 2^i
	//       - Whenever you use 2^i to build sum, XOR steps[i] into the running product
	//     - Return the product
	//   - Store these coefficients in out_coef[4] for outside use
}
