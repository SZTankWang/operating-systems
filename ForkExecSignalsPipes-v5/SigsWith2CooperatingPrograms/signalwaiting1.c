#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

volatile sig_atomic_t fl_s = 0;

void handle_sigusr1(int signum) {
	fl_s = 1;
}

// compile gcc signalwait1.c  and run ./a.out

int main(void) {
	int status;
	pid_t pid;

	if((pid = fork()) == -1) {
		perror("fork");
		exit(1);
	}

	signal(SIGUSR1, handle_sigusr1);

	/* Child */
	if (pid == 0) {
		if ((execl("./mysig", "mysig", NULL)) == -1) {
			perror("exec");
		}
	}
	/* Parent */
	else {
		printf("+++ Parent: My PID: %d\n", getpid());
		printf("+++ Parent: Waiting for my child to set its signal handler...\n");
		while(!fl_s)
			pause(); /* Wait For a Signal */
		printf("+++ Parent: My child has set its signal handler...\n");
		printf("+++ Parent: Sleeping\n");
		sleep(5);
		printf("+++ Parent: Finished Sleeping. Sending Signal USR1 to my child...\n");
		kill(pid, SIGUSR1);

		if(wait(&status) != pid) {
			perror("wait");
			exit(1);
		}

		if((status & 0377) == 0) {
			printf("+++ Parent: Child Exited Normally: %d\n", status >> 8);
		} else {
			printf("+++ Parent: Child Exited due to signal: %d\n", status & 0177);
		}
	}
	return 0;
}
