#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

double timing(struct timeval before);

int main() {

struct timeval tval_before, tval_after, tval_result;

gettimeofday(&tval_before, NULL);

// Some code you want to time, for example:
float time = 2.35 * 1e6;
usleep(time);

double res = timing(tval_before);


printf("my result: %f\n",res);
}



double timing(struct timeval before){
	struct timeval after, result;

	gettimeofday(&after,NULL);

	timersub(&after, &before, &result);

	double res = result.tv_sec + result.tv_usec/1e6;
	printf("INSIDE timing function: time elapsed is %f\n",res);
	return res;
};