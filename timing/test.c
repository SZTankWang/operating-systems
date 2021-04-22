#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

int main() {

  struct timeval  tv;
  gettimeofday(&tv, NULL);

double begin =
  (tv.tv_sec)  + (tv.tv_usec) / 1000 ;


useconds_t time = 1.5 * 1e6;
usleep(time);

 gettimeofday(&tv, NULL);

double end =
  (tv.tv_sec)  + (tv.tv_usec) / 1000 ;

  printf("Execution time %f\n", end - begin);
  return (0);
}