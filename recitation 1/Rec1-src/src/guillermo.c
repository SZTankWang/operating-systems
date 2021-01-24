#include <stdio.h>
#include <unistd.h>

main(){
int i=0;

while(1) {
	for( i=0; i<3; i++){
		fprintf(stdout,"Guillermo ");
		write(1,"Guillermo ",10);
		fflush(stdout);
		}
	fprintf(stdout,"!\n");	
	fprintf(stderr,"U r a smart fella\n");
	sleep(1);
	}
}
