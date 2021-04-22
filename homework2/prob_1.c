#include <stdio.h>

extern char _start;
extern char _etext;
extern char __data_start;
extern char _edata;
extern char __bss_start;
extern char _end;

int main(int argc, char const *argv[])
{
	/* code */
	int start_of_stack;

	printf("address of start of text section 0x%lx\n",(unsigned long)&_start);
	printf("address of end of text section 0x%lx\n",(unsigned long)&_etext);
	printf("address of start of data section 0x%lx\n",(unsigned long)&__data_start);
	printf("address of end of data section 0x%lx\n",(unsigned long)&_edata);
	printf("address of start of BSS section 0x%lx\n",(unsigned long)&__bss_start);
	printf("address of start of heap section 0x%lx\n",(unsigned long)&_end);
	printf("address of start of stack 0x%lx\n",(unsigned long)&start_of_stack);
	

	return 0;
}