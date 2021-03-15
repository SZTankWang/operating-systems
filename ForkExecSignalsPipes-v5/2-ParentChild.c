#include	<stdio.h>
#include	<sys/types.h>
#include	<unistd.h>
#include	<string.h>

#define 	BUFFSIZE 	10

int	main(void){
	int	i;
	static char	buffer[BUFFSIZE];

	if (fork()==0){
		strcpy(buffer,"CHILD\n");
		}
	else	{
		strcpy(buffer,"PARENT\n");
		}
	for (i=0;i<5;i++){
		 sleep(1);
		write(1,buffer,strlen(buffer));
		}
}
