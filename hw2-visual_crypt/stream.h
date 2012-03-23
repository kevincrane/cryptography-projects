// Kevin Crane
//   CS531 - Homework 2
//   stream.h

#ifndef STREAM_H
#define STREAM_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>


// A simple stream cipher
void stream(const char *pphrase, int len, unsigned char *out_buf);

#endif
