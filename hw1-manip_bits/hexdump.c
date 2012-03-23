// Kevin Crane
//   CS531 - Homework 1
//   hexdump.c

#include "hexdump.h"


// Prints plaintext output at end of row
void printLineOut(char lineOut[]) {
	// Print row overview in ASCII here
	for(unsigned int i=0; i<16; i++) {
		printf("%c", lineOut[i]);
	}
	printf("\n");
}

// Complete a hexdump of ASCII characters received
void hexdump(FILE *in) {
	char buf[1];
	char lineOut[16];
	unsigned char charRead;
	int numBytes = 0;
	int bytesInRow = 0;
	
	// Iterate through the entire input by bytes
	while(fread(buf, 1, 1, in) > 0) {
		charRead = buf[0];		
		
		if(bytesInRow == 0) {
			printf("%06x: ", numBytes);
		}
		
		// Processing of the byte read
		printf("%02x ", charRead);
		if(charRead >= 32 && charRead <= 126) {
			lineOut[bytesInRow] = charRead;
		} else if(charRead <= 31 || charRead == 127) {
			lineOut[bytesInRow] = '.';
		} else {
			lineOut[bytesInRow] = '~';
		}
		numBytes++;
		bytesInRow++;
		if(bytesInRow%8 == 0)
			printf(" ");
		
		// End of the row, print final row output
		if(bytesInRow == 16) {
			bytesInRow = 0;
			printLineOut(lineOut);
		}
	}
	
	// Account for excess space at the end of the input
	if(bytesInRow > 0) {
		for(unsigned int i = bytesInRow; i<sizeof(lineOut); i++) {
			// Set unused byte values to '--'
			printf("-- ");
			lineOut[bytesInRow] = ' ';
			numBytes++;
			bytesInRow++;
			if(bytesInRow%8 == 0)
				printf(" ");
				
			// End of the row
			if(bytesInRow == 16) {
				bytesInRow = 0;
				printLineOut(lineOut);
			}
		}
	}
	
	// PSEUDO-CODE
	// - Print out the current byte count as a 6-digit hex value
	// - Read each byte one by one and print it out as a hex value, putting spaces where needed
	// - After 16 bytes, print out the 16 hex values stored as characters
	// - Repeat until end of file
}
