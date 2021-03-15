#include <signal.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

int count=0;
void alrm_action(int);

void main(){
  int p[2];
  int pipe_size=0;
  char c='x';
  static struct sigaction act;

  // set up the signal handler
  act.sa_handler=alrm_action;
  sigfillset(&(act.sa_mask));

  if ( pipe(p) == -1) { perror("pipe call"); exit(1);}

  pipe_size=fpathconf(p[0], _PC_PIPE_BUF);

  printf("Maximum size of (atomic) write to pipe: %d bytes\n", pipe_size);
  printf("The respective POSIX value %d\n",_POSIX_PIPE_BUF);

  sigaction(SIGALRM, &act, NULL);

  while (1) {
       alarm(10);  		/* set alarm 			   */
       write(p[1], &c, 1);	/* do the writing of the character */
       alarm(0);		/* reset alarm  		   */
       if (++count % 4096 == 0 )/* report every 4kbytes written out*/
          printf("%d characters in pipe\n",count);
      }
}

void alrm_action(int signo){
  	printf("Alrm-Handler: write blocked after %d characters \n",count);
  	exit(0);
}
