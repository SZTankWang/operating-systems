CC=gcc
CFLAGS = -g -Wall

TARGET = prob_1

prob_1:prob_1.o
	$(CC) $(CFLAGS) -o prob_1 prob_1.o

prob_1.o:prob_1.c
	$(CC) $(CFLAGS) -c prob_1.c

make clean:
	rm *.o prob_1