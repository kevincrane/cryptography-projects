# Makefile for CS531 Homework 7


CC = g++
FLAGS = -Wall -g
OUTPUT = hw7

hw7: primes.o trialdiv.o millerrabin.o rndsearch.o maurer.o
	$(CC) $(FLAGS) primes.o trialdiv.o millerrabin.o rndsearch.o maurer.o hw7.c -o $(OUTPUT)
	
primes.o: primes.c primes.h
	$(CC) $(FLAGS) -c primes.c
	
trialdiv.o: trialdiv.c trialdiv.h
	$(CC) $(FLAGS) -c trialdiv.c
	
millerrabin.o: millerrabin.c millerrabin.h
	$(CC) $(FLAGS) -c millerrabin.c
	
rndsearch.o: rndsearch.c rndsearch.h
	$(CC) $(FLAGS) -c rndsearch.c
	
maurer.o: maurer.c maurer.h
	$(CC) $(FLAGS) -c maurer.c
	
	
clean:
	rm -f *.o $(OUTPUT)
