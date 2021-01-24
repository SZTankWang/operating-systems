#include	<stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include	"MyHeader.h"


/*
 * This set of calls creates an ordered (alphanumerically) 
 * linked list, and allows for the removal of individual elements, 
 * printing out, membership questions, and 
 * selective look-up (smallest/largest) element.
 *                              ad@di.uoa.gr 03/02
 */


struct item *OrderedCreateItem(struct item *p)
{
	p=NULL;
	return(p);
}

struct item *OrderedInsertItem(struct item *p, char *w)
{	struct	item	*aux=NULL;
	
	if ( p==NULL ) {
                p = (struct item *) malloc (sizeof(struct item)+1);
                p->word = (char *) malloc(sizeof(w)+1);
                strcpy(p->word,w);
                p->next = NULL;
		return(p);
                }                 
	else if ( strcmp(p->word, w) == 0 ) {
		printf("Word %s is already in the List \n",w);
		return(p);
		}
	else if ( strcmp (p->word, w) < 0 ) {	
		p->next = OrderedInsertItem (p->next, w);
		return(p);
		}
	else	{
		aux = (struct item *) malloc (sizeof(struct item)+1);
                aux->word = (char *) malloc(sizeof(w)+1);
                strcpy(aux->word,w);
                aux->next = NULL;
		aux->next = p;
		return(aux);
                }               
}

struct item *OrderedDeleteItem(struct item *p, char *w)
{	struct	item	*aux=NULL, *throwaway=NULL;

	aux=p;
	if (aux==NULL) 
		return (NULL);
	else {
		if ( strcmp(aux->word, w) == 0 ){
			throwaway=aux;
			aux = aux->next;
			free (throwaway) ;
			throwaway = NULL;
			return(aux);
			}
		else if ( strcmp(aux->word, w) > 0 ) {
			printf("Item seems not in the list - outa here!\n");
			return(aux);
			}
		else	{
			aux->next=OrderedDeleteItem(aux->next, w);
			return(aux);
			}
	}
}

char *GetSmallestItem(struct item *p)
{       struct  item    *aux=NULL;
        char            *w=NULL;
 
        aux=p;
        if (aux==NULL)
                return(NULL);
        else    {
                w = (char *)malloc(sizeof(aux->word)+1);
                strcpy(w, aux->word);
                printf("first word is %s \n",w);
                return(w);
                }
}                      

char *GetLargestItem(struct item *p)
{       struct  item    *aux=NULL;
        char            *w=NULL;
 
        aux=p;
        if (aux==NULL)
                return(NULL);
        else    {
                while (aux->next!=NULL)
                        aux=aux->next;
                w = (char *)malloc(sizeof(aux->word)+1);
                strcpy(w, aux->word);
                return(w);
                }
}                    

int	IsMemberOfOrderedList(struct item *p, char *w)
{	struct	item	*aux=NULL;

	if (p==NULL){
		printf("Search for Nothing - List is Empty! \n");
		return(NO);
		}
	
	aux = p;
	while (aux!=NULL){
		if ( strcmp( aux->word, w) == 0 )
			return(YES);
		else if ( strcmp( aux->word, w) > 0 )
			return(NO);
		else	;
		aux=aux->next;
		}
	return(NO);
}
		


void	OrderedPrintItem(struct item *p)
{ 
	struct item *aux=NULL;
	
	if ( p == NULL ){
		printf("List is Empty of Items\n");
		return;
		}
	aux = p;
	while (aux!= NULL){
		printf("Element %s\n",aux->word);
		aux=aux->next;
		}
}
