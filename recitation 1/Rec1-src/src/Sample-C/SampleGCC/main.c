#include	<stdio.h>
#include 	<string.h>
#include 	<stdlib.h>
#include	"MyHeader.h"
#include	"LinkedList.h"
#include	"OrderedLinkedList.h"

/* This is a comment on March 1st 2016 */

/*
 * This program opens a file (provided as inline parameter in argv[1],
 * stores the words (of the file) in an Ordered Linked List, 
 * deletes a couple of elements, and then writes the elements of the  
 * list in reverse order in a file with postfix ".out"
 * 				ad@di.uoa.gr 03/02
 */

int 	main(int argc, char *argv[])
/* argc, argv: designate inline parameters */
{	
	struct item *List = NULL;
	char	myword[MAXSIZE], buffer[MAXSIZE];
	int	myboolean = TRUE;
 	FILE 	*fp=NULL, *wfp=NULL;

	/* do an initial testing of inline parameters */
	if (argc!=2){
		printf("expected syntax: myexe inputfile\n");
		exit(1);
		}
	else	{
		if ( (fp=fopen(argv[1],"r"))==NULL ){
			/* if you cannot open the file exit! */
			printf("Unknown File...\n");
                	exit(1);
                	} 
		else	{
			/* start with the creation of the list */
			List = CreateItem(List);
			/* List = OrderedCreateItem(List); */

			while (fscanf(fp,"%s",myword)!= EOF ) {
				printf("%s\n",myword);
				/* enter words into the list */
				List = OrderedInsertItem (List, myword);
				}
			fclose(fp);		
			/* print the conent of the list out */
			OrderedPrintItem(List);
			}
		}
	/* delete the smallest element in the list */
	List = OrderedDeleteItem(List, GetSmallestItem(List));
	printf("Have deleted the Smallest element of the List\n");
	OrderedPrintItem(List);

	/* delete the largest element in the list */
	List = OrderedDeleteItem(List, GetLargestItem(List));
	printf("Have deleted the Largest element of the List\n");
	OrderedPrintItem(List);

	/* write the content of the list in reverse order in the 
	   file named "argv[1].out"
	*/
	strcpy(buffer,argv[1]); 
	strcat(buffer,".out");
	if ( (wfp=fopen(buffer,"w"))==NULL ){		
		printf("Could not open file - exit!\n");
               	exit(1);
		} 
	else	{
		while ( myboolean == TRUE ){
			if( GetLargestItem(List) == NULL )
				myboolean=FALSE;
			else 	{
				strcpy(myword,GetLargestItem(List));
				printf("LARGEST found is %s\n",myword);
				fprintf(wfp,"%s\n",myword);
				List = OrderedDeleteItem(List, myword); 
				printf("The list is as follows: \n");
				OrderedPrintItem(List);
				}
			}
		fclose(wfp);
		}
	printf("End of this Sample main() \n");
	exit(1);
}
