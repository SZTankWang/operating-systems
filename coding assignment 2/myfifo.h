#ifndef MYFIFO_H
#define MYFIFO_H

class myfifo{
public:
	myfifo(char * pathname,int flag);
	char * pathname;
	int flag;//signify if it has been read, 0 for not, 1 for true
	int fd;
};

#endif