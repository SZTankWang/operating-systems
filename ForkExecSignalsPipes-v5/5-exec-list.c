#include	<stdio.h>
#include	<sys/types.h>
#include	<stdlib.h>
#include 	<unistd.h>

/*
 * if "cat" comes off "/bin/cat" * run the following as
 * 		"5-exec-list 5-exec-list.c"
 */

int  	main(int argc, char *argv[]){

	if (argc >1){
		execlp("cat", "cat", "-T", "-E", "-n", argv[1], (char *)NULL );
		perror("exec failure.. ");
		exit(1);
		}
	fprintf(stderr,"Usage: %s file \n", *argv);
	exit(1);
}
