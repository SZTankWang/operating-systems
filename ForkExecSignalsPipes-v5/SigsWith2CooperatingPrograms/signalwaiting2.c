#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

// compile gcc -c signalwaiting2.c -o mysig

volatile sig_atomic_t fl_s = 0;

void handle_sigusr1(int signum) {
	fl_s = 1;
	printf("/\\/\\ Child Process %d: Received USR1\n", getpid());
}

/* Executable: mysig */
int main(void){
	printf("--- Child: My PID: %d, PPID: %d\n", getpid(), getppid());
	signal(SIGUSR1, handle_sigusr1);

	/* Send signal to parent to notify that i have set my signal handler */
	kill(getppid(), SIGUSR1);

	printf("--- Child Waiting for SIGUSR1\n");
	while(!fl_s)
		pause(); /* Wait For a Signal */
	printf("--- Child Finished Waiting --> exiting...\n");
	return 0;
}
