SOURCE1		= signalwaiting1
SOURCE2		= signalwaiting2
CFLAGS		= -c
CC		= gcc
OUT1		= sig1-calling-mysig
OUT2		= mysig
LIBS		= 
INCS		= 

all: $(OUT1) $(OUT2) 

$(OUT1): $(SOURCE1).c
	gcc -o $(OUT1) $(SOURCE1).c

$(OUT2): $(SOURCE2).c
	gcc -o $(OUT2) $(SOURCE2).c

clean:	
	rm -f $(SOURCE1).o $(OUT1) $(SOURCE2) $(OUT2) a.out;
	touch $(SOURCE1).c $(SOURCE2).c
