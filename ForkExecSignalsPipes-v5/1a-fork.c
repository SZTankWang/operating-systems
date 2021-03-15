#include	<stdio.h>
#include	<sys/types.h>
#include	<unistd.h>
#include	<string.h>
#include        <stdlib.h>

int	main(void){
        pid_t retval;

        retval=fork();
        if (retval== -1){
                perror("Failed to fork\n");
                exit(1);
                }

        if (retval==0){
                printf("I am the child process  with ID: %lu \n",(long)getpid());
                printf("The value of the retval is: %d\n",retval);
                }
        else    {
                printf("I am the parent process with ID: %lu \n",(long)getpid());
                printf("The value of ertval is: %d\n",retval);
                }
        return(0);
}
