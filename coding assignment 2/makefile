CC = g++
CFLAGS = -g -Wall
RM = rm

all:myhie bubble

myhie:main.o myfifo.o
	$(CC) $(CFLAGS) -o myhie main.o myfifo.o

main.o:main.cpp myfifo.h
	$(CC) $(CFLAGS) -c main.cpp


myfifo.o:myfifo.cpp myfifo.h
	$(CC) $(CFLAGS) -c myfifo.cpp

Taxpayer.o:Taxpayer.cpp Taxpayer.h
	$(CC) $(CFLAGS) -c Taxpayer.cpp 	

bubble:bubbleSort.o Taxpayer.o
	$(CC) $(CFLAGS) -o bubble bubbleSort.o Taxpayer.o

bubbleSort.o:bubbleSort.cpp Taxpayer.h
	$(CC) $(CFLAGS) -c bubbleSort.cpp
	
clean:
	$(RM) *.o myhie bubble
