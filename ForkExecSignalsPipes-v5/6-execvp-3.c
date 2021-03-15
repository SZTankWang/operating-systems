#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(){
  pid_t  pid;
  int  status, exit_status;
  char *buff[2];

  if ( (pid = fork()) < 0 ){
	perror("fork failed"); exit(1);}

  if (pid==0){
     buff[0]=(char *)malloc(12); strcpy(buff[0],"date");
     printf("%s\n",buff[0]); buff[1]=(char *)NULL;
     printf("I am the child process %d ",getpid());
     printf("and will be replaced by 'date'\n");
     execvp("date",buff);
     exit(5); /* exit with non-zero value */
     }
  else {
     printf("Hello I am in parent process %d with child %d\n", getpid(), pid);
     if ( wait(&status)!= pid ) { perror("wait"); exit(1);}
     printf("Child terminated with exit code %d\n", status>>8);
     }
}
