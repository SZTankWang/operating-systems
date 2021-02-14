CC = g++
CFLAGS = -g -Wall
RM = rm


mvote:main.o hashTable.o posCodeList.o posNode.o voter.o voted.o info.o helper.o
	$(CC) $(CFLAGS) -o mvote main.o hashTable.o posCodeList.o posNode.o voter.o voted.o info.o helper.o

main.o:main.cpp hashTable.h helper.h
	$(CC) $(CFLAGS) -c main.cpp

hashTable.o:hashTable.cpp hashTable.h
	$(CC) $(CFLAGS) -c hashTable.cpp

posCodeList.o:posCodeList.cpp posCodeList.h
	$(CC) $(CFLAGS) -c posCodeList.cpp 

posNode.o:posNode.cpp posNode.h
	$(CC) $(CFLAGS) -c posNode.cpp

voter.o:voter.cpp voter.h
	$(CC) $(CFLAGS) -c voter.cpp

voted.o:voted.cpp voted.h
	$(CC) $(CFLAGS) -c voted.cpp

info.o:info.cpp info.h
	$(CC) $(CFLAGS) -c info.cpp

helper.o:helper.cpp helper.h
	$(CC) $(CFLAGS) -c helper.cpp	

clean:
	$(RM) *.o