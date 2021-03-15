#include	<stdio.h>
#include	<sys/types.h>
#include	<unistd.h>
#include	<string.h>

#define 	BUFFSIZE	20

int	main(void){
	int		i=0;
	static char     bufferA[BUFFSIZE];
	static char     bufferB[BUFFSIZE];

	if (fork()==0){
		strcpy(bufferA,"In Child Process \n");
		write(1,bufferA,strlen(bufferA));
		}
	else	{
		strcpy(bufferB,"In Parent Process \n");
		write(1,bufferB,strlen(bufferB));
		}

}
