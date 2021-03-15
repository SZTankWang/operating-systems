#include	<stdio.h>
#include	<sys/types.h>
#include	<unistd.h>
#include	<string.h>

int	main(void){

	fork();

	printf("hee\n");

	fork();

	printf("ha\n");

	fork();

	printf("ho\n");

}

/*

Processes 142 >a.out
hee
ha
ho
ho
ha
ho
hee
ha
ho
ho
ha
ho
ho
ho

*/
