# include <stdio.h>
# include <signal.h>
#include <unistd.h>

void foo(int);

int main (){
	int i;
	signal (SIGINT , foo);

	for (i=0; i <5;i ++){
		printf(" hello \n");
		sleep (1) ;
		}
	}

void foo( int signum ){ /* no explicit call to function f in main()*/
	signal (SIGINT , foo);
	/* re - establish disposition of the signal SIGINT */

	printf ("OUCH !\n");
	}
