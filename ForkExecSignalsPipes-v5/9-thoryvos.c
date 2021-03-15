#include	<stdio.h>
#include	<sys/types.h>
#include	<unistd.h>
#include	<string.h>
#include 	<stdlib.h>


#define 	BUFFSIZE 	10

int	main(void){
	int	i, k;
	static char	buffer[BUFFSIZE];
	char 	*p=NULL;
	
	for(;;) {
		i= (i++)%8096;
		p=(char *)malloc(i*3*1024*1024*1024);
		for(k=0;k<2000000;k++) ;
		free(p);
		p=NULL;
		}

}



