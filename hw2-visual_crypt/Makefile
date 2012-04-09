# Makefile for CS531 Homework 2


CC = g++
FLAGS = -Wall
OUTPUT = hw2
OPENSSL_LINK = -lcrypto

hw2: stream.o encrypt.o merge.o decrypt.o hw2.c
	$(CC) $(FLAGS) stream.o encrypt.o merge.o decrypt.o hw2.c $(OPENSSL_LINK) -o $(OUTPUT)
	
stream.o: stream.c stream.h
	$(CC) $(FLAGS) $(OPENSSL) -c stream.c
	
encrypt.o: encrypt.c encrypt.h
	$(CC) $(FLAGS) -c encrypt.c

merge.o: merge.c merge.h
	$(CC) $(FLAGS) -c merge.c
	
decrypt.o: decrypt.c decrypt.h
	$(CC) $(FLAGS) -c decrypt.c
	
	
clean:
	rm -f *.o *.1.pbm *.2.pbm *.stream $(OUTPUT)
