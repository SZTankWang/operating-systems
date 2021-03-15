#include	<stdio.h>
#include	<sys/types.h>
#include	<stdlib.h>
#include 	<unistd.h>

/* for example, run it as follows:
 *  "4-SingleExecvp cat -n 4-SingleExecvp.c"
 */

int  	main(int argc, char *argv[]){

	execvp(argv[1], &argv[1]);
	perror("exec failure ");
	exit(1);
}
