// Kevin Crane
//   CS531 - Homework 2
//   stream.c

#include <stdio.h>
#include <stdlib.h>
#include <openssl/md5.h>

#include "stream.h"

#define BYTES_PER_ITER MD5_DIGEST_LENGTH/2


// A simple stream cipher; outputs 'len' bytes of MD5 stream cipher
void stream(const char *pphrase, int len, unsigned char *out_buf=NULL) {

	unsigned char md5_buf[MD5_DIGEST_LENGTH];
	int str_len=strlen(pphrase)+2+MD5_DIGEST_LENGTH;
	char *s=(char*)malloc(str_len+1);
	int i=0;
	int bytes_out = 0;
	
	unsigned char* out_ptr = out_buf;

	// Initial MD5 hash of passphrase
	MD5((const unsigned char *)pphrase, strlen(pphrase), md5_buf);
	while(len > 0) {
		// Assemble iteration key into buffer s
		// {Previous MD5 Hash} + {[00,99]} + {passphrase}
		sprintf(&s[MD5_DIGEST_LENGTH], "%02d%s", i, pphrase);
		memcpy(s, md5_buf, MD5_DIGEST_LENGTH);
		
		// Take MD5 hash of this iteration key
		MD5((const unsigned char *)s, str_len, md5_buf);
		
		// How many bytes to print (up to 8)?
		if(len - BYTES_PER_ITER >= 0) {
			bytes_out = BYTES_PER_ITER;
			len -= BYTES_PER_ITER;
		} else {
			bytes_out = len;
			len = 0;
		}
		
		// Determine location of output (stdout or out_buf)
		if(out_buf == NULL) {
			// Default behavior if 'stream' prog is run
			fwrite(md5_buf, 1, bytes_out, stdout);
		} else {
			// Print output to out_buf instead for PBM encryption
			memcpy(out_ptr, md5_buf, bytes_out);
			out_ptr += bytes_out;
		}
		
		// Reset the middle int value if needed
		if(++i == 100) 
			i = 0;
	}
	free(s);
	
	
	// PSEUDO-CODE
	// - Allocate enough memory to hold the whole stream cipher
	// - Take the MD5 hash of the original passphrase
	// - Concat this hash string with an integer {0-99}, and the passphrase again
	// - Take the MD5 of this new string; first 8 bytes are the beginning of the stream cipher
	// - Output these 8 bytes to either stdout or a char* out_buf
	// - Repeat for as long as needed to fill length of cipher
}
