#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#define READ 0
#define WRITE 1

void 	main(int argc, char *argv[]){
	int fd1[2], fd2[2], filedesc1= -1;
	int status;
	char myinputparam[20];
	pid_t pid; 

	// create a number of file(s)/pipe(s)/etc

	if ( (filedesc1=open("MytestFile", O_WRONLY|O_CREAT, 0666)) == -1){
		perror("file creation"); exit(1);
		}
	if ( pipe(fd1) == -1 ) {
		perror("pipe"); exit(1);
		}
	if ( pipe(fd2)== -1 ) {
                perror("pipe"); exit(1);
                }

	if ( (pid=fork()) == -1){
		perror("fork"); exit(1);
		}

	if ( pid!=0 ){          // parent process - closes off everything
		close(filedesc1);
		close(fd1[READ]); close(fd1[WRITE]);
		close(fd2[READ]); close(fd2[WRITE]);
		close(0); close(1); close(2);
		if (wait(&status)!=pid){
			perror("Waiting for child\n"); exit(1);
			}
		else 	printf("Just synched with child\n");
		}
	else {
		printf("filedesc1=%d\n", filedesc1);
		printf("fd1[READ]=%d, fd1[WRITE]=%d,\n",fd1[READ], fd1[WRITE]);
		printf("fd2[READ]=%d, fd2[WRITE]=%d\n", fd2[READ], fd2[WRITE]);
		dup2(fd2[WRITE], 11);
		execlp(argv[1], argv[1], "11", NULL);
		perror("execlp");
	}
}
