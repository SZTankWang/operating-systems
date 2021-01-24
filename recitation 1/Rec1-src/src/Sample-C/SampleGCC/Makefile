#
# In order to execute this "Makefile" just type "make"
#	A. Delis (ad@di.uoa.gr)
#

OBJS 	= main.o LinkedList.o OrderedLinkedList.o
SOURCE	= main.c LinkedList.c OrderedLinkedList.c
HEADER  = MyHeader.h LinkedList.h OrderedLinkedList.h
OUT  	= myexe
CC	= gcc
FLAGS   = -g -c -pedantic -ansi  -Wall
# -g option enables debugging mode 
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@

# create/compile the individual files >>separately<< 
main.o: main.c
	$(CC) $(FLAGS) main.c

LinkedList.o: LinkedList.c
	$(CC) $(FLAGS) LinkedList.c

OrderedLinkedList.o: OrderedLinkedList.c
	$(CC) $(FLAGS) OrderedLinkedList.c

# clean house
clean:
	rm -f $(OBJS) $(OUT)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
