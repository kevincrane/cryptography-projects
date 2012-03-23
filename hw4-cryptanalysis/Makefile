# Makefile for CS531 Homework 4


CC = g++
FLAGS = -Wall
OUTPUT = hw4
OPENSSL_LINK = -lcrypto

hw4: keygen.o crypt.o invkey.o histo.o solve.o hw4.c
	$(CC) $(FLAGS) keygen.o crypt.o invkey.o histo.o solve.o hw4.c -o $(OUTPUT)
	
keygen.o: keygen.c
	$(CC) $(FLAGS) -c keygen.c
	
crypt.o: crypt.c
	$(CC) $(FLAGS) -c crypt.c
	
invkey.o: invkey.c
	$(CC) $(FLAGS) -c invkey.c

histo.o: histo.c
	$(CC) $(FLAGS) -c histo.c
	
solve.o: solve.c
	$(CC) $(FLAGS) -c solve.c

	
clean:
	rm -f *.o *.keys $(OUTPUT)
