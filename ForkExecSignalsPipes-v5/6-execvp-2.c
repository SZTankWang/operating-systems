#include	<stdio.h>
#include	<sys/types.h>
#include	<stdlib.h>
#include 	<unistd.h>

/*
 * invoke this little program as: "./6-execvp-2 cat prog6.c"
 *
 * then try to run it as: "6-execvp-2 cat -n 6-execvp-2.c" (what happens then?)
 */

int  	main(int argc, char *argv[]){

	if ( execvp(argv[1], &argv[1]) == -1 ) {
		perror("exec failure ");
		exit(1);
		}
	else	{
		exit(1);
		}
}
