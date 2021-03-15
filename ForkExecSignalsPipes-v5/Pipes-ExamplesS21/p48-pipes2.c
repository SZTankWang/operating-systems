#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MSGSIZE 16

char *msg1="Buenos Dias! #1";
char *msg2="Buenos Dias! #2";
char *msg3="Buenos Dias! #3";

void main(){
  char inbuf[MSGSIZE];
  int p[2], i=0, rsize=0, status;
  pid_t pid;

  if (pipe(p)==-1) { perror("pipe call"); exit(1);}

  switch(pid=fork()){
  case -1: perror("fork call"); exit(2);
  case  0: // child is writing
	   close(p[0]);                 // close off the read end..
           printf("In child - I am sleeping it off a bit... \n");
	   sleep(4);
           write(p[1],msg1,MSGSIZE);
           write(p[1],msg2,MSGSIZE);
           write(p[1],msg3,MSGSIZE);
	   break;
  default: // parent is reading
	   close(p[1]);                 // close off the write end
           for (i=0;i<3;i++){
 		rsize=read(p[0],inbuf,MSGSIZE);
		printf("%.*s\n",rsize,inbuf);
		}
	    wait(&status);
  }
  exit(0);
}
