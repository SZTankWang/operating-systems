#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define  READ 0
#define  WRITE 1

int main(int argc, char *argv[]){
pid_t  pid; 
int fd[2], bytes;

if ( argc!= 3) {printf("\n./p53-pipes-dup prog1 prog2\n"); exit(23);}

if (pipe(fd) == -1){ perror("pipe"); exit(1); }

if ( (pid = fork()) == -1 ){ perror("fork"); exit(1); }

if ( pid != 0 ){    // parent and writer
	close(fd[READ]);
     	dup2(fd[WRITE],1);
     	close(fd[WRITE]);
     	execlp(argv[1], argv[1], NULL);
     	perror("execlp");
     	}
else{                // child and reader
     	close(fd[WRITE]); 
     	dup2(fd[READ],0);
     	close(fd[READ]);
     	execlp(argv[2],argv[2],NULL);
     	perror("execlp - ");
     	}
}
