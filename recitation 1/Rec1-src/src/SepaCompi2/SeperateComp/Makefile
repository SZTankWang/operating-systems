
# Makefile for executable adjust

# *****************************************************
# Parameters to control Makefile operation

CC = gcc
CFLAGS = -ansi -pedantic -Wall -g

# ****************************************************
# Entries to bring the executable up to date

adjust: adjust.o grades.o
	$(CC) $(CFLAGS) -o adjust adjust.o grades.o
	
adjust.o: adjust.c grades.h
	$(CC) $(CFLAGS) -c adjust.c

grades.o: grades.c grades.h	 $(CC) $(CFLAGS) -c grades.c

clean: 
	rm -f grades.o adjust.o adjust

