# Makefile for CS531 Homework 7


CC = g++
FLAGS = -Wall -g
OUTPUT = hw7
OPENSSL_LINK = -lcrypto

hw7: primes.o trialdiv.o millerrabin.o rndsearch.o maurer.o hw7.c
	$(CC) $(FLAGS) primes.o trialdiv.o millerrabin.o rndsearch.o maurer.o hw7.c $(OPENSSL_LINK) -o $(OUTPUT)
	
primes.o: primes.c primes.h
	$(CC) $(FLAGS) -c primes.c
	
trialdiv.o: trialdiv.c trialdiv.h
	$(CC) $(FLAGS) $(OPENSSL_LINK) -c trialdiv.c
	
millerrabin.o: millerrabin.c millerrabin.h
	$(CC) $(FLAGS) $(OPENSSL_LINK) -c millerrabin.c
	
rndsearch.o: rndsearch.c rndsearch.h
	$(CC) $(FLAGS) -c rndsearch.c
	
maurer.o: maurer.c maurer.h
	$(CC) $(FLAGS) -c maurer.c
	
	
clean:
	rm -f *.o $(OUTPUT)
