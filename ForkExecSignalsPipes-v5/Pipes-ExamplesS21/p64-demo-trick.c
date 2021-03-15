#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>


#define SENTINEL -1

void	main(){
	pid_t pid; 
	int fd=SENTINEL;

	printf("About to run who into a file (in a strange way!)\n");

	if ( (pid=fork())== SENTINEL){
		perror("fork"); exit(1);
		}

	if ( pid == 0 ){
		close(1);   /* change this to 0 say - any effect? */
		fd=creat("userlist", 0644);
		fprintf(stderr,"the fd is: %d\n",fd); fflush(stderr);
		execlp("who","who",NULL);
		perror("execlp");
		exit(1);
		}
	
	if ( pid != 0){
		wait(NULL);
		printf("Done running who - results in file \"userlist\"\n");
		}
}
