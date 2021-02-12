CC = g++
CFLAGS = -g -Wall
RM = rm


main:main.o hashTable.o posCodeList.o posNode.o voter.o
	$(CC) $(CFLAGS) -o main main.o hashTable.o posCodeList.o posNode.o voter.o 

main.o:main.cpp
	$(CC) $(CFLAGS) -c main.cpp

hashTable.o:hashTable.cpp hashTable.h voter.h
	$(CC) $(CFLAGS) -c hashTable.cpp

posCodeList.o:posCodeList.cpp posCodeList.h voter.h posNode.h 
	$(CC) $(CFLAGS) -c posCodeList.cpp 

posNode.o:posNode.cpp voter.h
	$(CC) $(CFLAGS) -c posNode.cpp

voter.o:voter.cpp posCodeList.h voter.h posNode.h
	$(CC) $(CFLAGS) -c voter.cpp

clean:
	$(RM) *.o main
