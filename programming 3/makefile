CC=gcc
CFLAGS = -I -Wall -pthread -g

all:chef maker

chef:chef.o myUtil.o
	$(CC) $(CFLAGS) -o chef chef.o myUtil.o
chef.o:chef.c info.h myUtil.h
	$(CC) $(CFLAGS) -c chef.c 

maker:maker.o myUtil.o
	$(CC) $(CFLAGS) -o maker maker.o myUtil.o

maker.o:maker.c info.h myUtil.h
	$(CC) $(CFLAGS) -c maker.c 

myUtil.o:myUtil.h info.h
	$(CC) $(CFLAGS) -c myUtil.c 


clean:
	rm chef maker *.o