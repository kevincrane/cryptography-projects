// Kevin Crane
//   CS531 - Homework 6
//   tablecheck.c

#include "tablecheck.h"


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


// Line beginning with 'S=' should contain a permutation of bytevals 00 to ff, inclusive
bool check_s(FILE* table) {
	const char* header = "S=";
	char* line_buf = (char*)calloc(1,640);
	int val_count[256] = { 0 };
	
	// Find and read the correct line
	fseek(table, 0, SEEK_SET);
	while(strncmp(line_buf, header, strlen(header)) != 0) {
		if(!fgets(line_buf, 640, table)) {
			fprintf(stderr, "ERROR: line for '%s' not found in tablefile.\n", header);
			return false;
		}
	}
	
	// Iterate through each two-hexchar byte of the tablefile, add to val_count
	char temp_val[3];
	int temp_index;
	for(unsigned int i=strlen(header); i<strlen(line_buf)-1; i+=2) {
		if(!is_hex_char(line_buf[i]) || !is_hex_char(line_buf[i+1])) {
			fprintf(stderr, "ERROR: invalid val found for S-Blocks. Must have even number of chars, all hex vals.\n");
			return false;
		}
		
		temp_val[0] = line_buf[i];
		temp_val[1] = line_buf[i+1];
		temp_val[2] = ' ';
		temp_index = strtol(temp_val, NULL, 16);
		val_count[temp_index] += 1;
	}
	
	// Iterate through all of the values read to make sure each happens exactly once
	for(int i=0; i<256; i++) {
		// Byte not found exactly once
		if(val_count[i] != 1) {
			char val[3];
			sprintf(val, "%02x", i);
			if(val_count[i] < 1) {
				fprintf(stderr, "ERROR: value '%s' was not found in the tablefile.\n", val);
				return false;
			} else if(val_count[i] > 1) {
				fprintf(stderr, "ERROR: value '%s' was found more than once in the tablefile.\n", val);
				return false;
			}
		}
	}
	
	free(line_buf);
	
	// Everything checks out
	return true;
}


// Lines beginning with 'P=' and 'INVP=' should each contain 8 hex chars, corresponding to 4 bytes
//	Each byte corresponds to the coefficient of a polynomial (x^3 + .. + x^0)
bool check_p(FILE* table) {
	
	const char* header;
	char* p_buf = (char*)calloc(1,16);
	char* invp_buf = (char*)calloc(1,16);
	unsigned char* out_buf = (unsigned char*)calloc(1,4);
	unsigned char p1[4];
	unsigned char p2[4];
	
	// Find and read the correct lines for P and INVP, strip annoying newline char
	header = "P=";
	fseek(table, 0, SEEK_SET);
	while(strncmp(p_buf, header, strlen(header)) != 0) {
		if(!fgets(p_buf, 16, table)) {
			fprintf(stderr, "ERROR: line for '%s' not found in tablefile.\n", header);
			return false;
		}
		p_buf[ strlen(p_buf)-1 ] = '\0';
	}
	header = "INVP=";
	fseek(table, 0, SEEK_SET);
	while(strncmp(invp_buf, header, strlen(header)) != 0) {
		if(!fgets(invp_buf, 16, table)) {
			fprintf(stderr, "ERROR: line for '%s' not found in tablefile.\n", header);
			return false;
		}
		invp_buf[ strlen(invp_buf)-1 ] = '\0';
	}
	
	// Split P and INVP into 4 bytes each
	char temp_val[3];
	if(strlen(p_buf) != (strlen("P=")+8)) {
		fprintf(stderr, "ERROR: polynomial must consist of exactly 8 characters.\n");
		return false;
	} else if(strlen(invp_buf) != (strlen("INVP=")+8)) {
		fprintf(stderr, "ERROR: inverse polynomial must consist of exactly 8 characters.\n");
		return false;
	}
	for(int i=0; i<4; i++) {
		temp_val[0] = *(p_buf+strlen("P=") + i*2);
		temp_val[1] = *(p_buf+strlen("P=") + i*2+1);
		temp_val[2] = ' ';
		if(!is_hex_char(temp_val[0]) || !is_hex_char(temp_val[1])) {
			fprintf(stderr, "ERROR: all characters in polynomial must be hex values.\n");
			return false;
		}
		p1[i] = strtol(temp_val, NULL, 16);
		
		temp_val[0] = *(invp_buf+strlen("INVP=") + i*2);
		temp_val[1] = *(invp_buf+strlen("INVP=") + i*2+1);
		temp_val[2] = ' ';
		if(!is_hex_char(temp_val[0]) || !is_hex_char(temp_val[1])) {
			fprintf(stderr, "ERROR: all characters in inverse polynomial must be hex values.\n");
			return false;
		}
		p2[i] = strtol(temp_val, NULL, 16);
	}
	
	// Perform MODPROD operation on the two polys read, verify that they are inverses
	get_modprod(p1, p2, out_buf);
	
	if((out_buf[0] != 0x00) || (out_buf[1] != 0x00) || (out_buf[2] != 0x00) || (out_buf[3] != 0x01)) {
		fprintf(stderr, "ERROR: modprod coefficients were {%02x}{%02x}{%02x}{%02x}; invalid inverse poly.\n", 
				out_buf[0], out_buf[1], out_buf[2], out_buf[3]);
		return false;
	}
	
	free(p_buf);
	free(invp_buf);
	free(out_buf);
	
	// Everything checks out
	return true;
}


// Check the integrity of tablefile
bool tablecheck(FILE *table) {

	// Check S vals
	if(!check_s(table)) {
		// S vals are invalid
		fprintf(stderr, "ERROR: tablecheck failed for S (Substitution) values.\n");
		return false;
	}
	
	// Check P/INVP vals
	if(!check_p(table)) {
		// P/INVP vals are invalid
		fprintf(stderr, "ERROR: tablecheck failed for P/INVP values.\n");
		return false;
	}
	
	return true;
	
	// PSEUDO-CODE
	// - Verify S Values
	//   - Convert every pair of hexchars into a byte value
	//   - Increment value for that particular byte in 256-val array
	//   - Verify that each value in array occurs exactly once
	// - Very P/INVP Values
	//   - Check that each of the two polynomials are the proper length and only hexchars
	//   - Plug these two polynomials into the modprod function and make sure that the 
	//     output is {00}{00}{00}{01}
}
