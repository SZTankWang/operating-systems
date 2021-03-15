#ifndef UTILITY_H
#define UTILITY_H

#include "Taxpayer.h"
#include "myfifo.h"


/*
function: joinPayerInfo
join all the info fields of a taxpayer into a char buffer for writing in the fifo
*/
char * joinPayerInfo(Taxpayer * payer);


/*
function: catStrWithSpaec
strcat a string to another, add a space at the end
*/
void catStrWithSpace(char * dest, char * src);
#endif // UTILITY_H_INCLUDED
