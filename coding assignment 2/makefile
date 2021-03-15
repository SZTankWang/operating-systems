CC = g++
CFLAGS = -g -Wall
RM = rm

all:myhie sorter

myhie:main.o myfifo.o Taxpayer.o algorithm.o utility.o
	$(CC) $(CFLAGS) -o myhie main.o myfifo.o Taxpayer.o algorithm.o utility.o

main.o:main.cpp myfifo.h Taxpayer.h algorithm.h
	$(CC) $(CFLAGS) -c main.cpp


myfifo.o:myfifo.cpp myfifo.h
	$(CC) $(CFLAGS) -c myfifo.cpp

Taxpayer.o:Taxpayer.cpp Taxpayer.h
	$(CC) $(CFLAGS) -c Taxpayer.cpp 	

sorter:sorter.o Taxpayer.o algorithm.o utility.o
	$(CC) $(CFLAGS) -o sorter sorter.o Taxpayer.o algorithm.o utility.o

sorter.o:sorter.cpp Taxpayer.h algorithm.h
	$(CC) $(CFLAGS) -c sorter.cpp

algorithm.o:algorithm.cpp algorithm.h
	$(CC) $(CFLAGS) -c algorithm.cpp

utility.o:utility.cpp utility.h Taxpayer.h
	$(CC) $(CFLAGS) -c utility.cpp
	
clean:
	$(RM) *.o myhie sorter
