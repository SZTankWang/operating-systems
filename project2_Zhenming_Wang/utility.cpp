#include <string.h>
#include <cstdlib>
#include <stdio.h>


#include <cstddef>

#include "utility.h"
#include "Taxpayer.h"



/*
join all the info fields of a taxpayer into a char buffer for writing in the fifo
*/

char * joinPayerInfo(Taxpayer * payer){
    char * temp = (char *)malloc(sizeof(char)*256);
    char RID[16];
    int RID_INT = payer->getRid(); 
    sprintf(RID,"%d",RID_INT);
    strcpy(temp,RID);
    strcat(temp," ");
    catStrWithSpace(temp,payer->getFirstName());
    catStrWithSpace(temp,payer->getLastName());

    char depd[16];
    int depd_INT = payer->getNumOfDepd();
    sprintf(depd,"%d",depd_INT);
    catStrWithSpace(temp,depd);

    char income[16];
    double income_INT = payer->getIncome();
    sprintf(income,"%f",income_INT);
    catStrWithSpace(temp,income);

    char posCode[16];
    int pos_INT = payer->getPosCode();
    // cout<<"*****INSIDE joinInfo<<<<poscode is "<<pos_INT<<endl;
    sprintf(posCode,"%d",pos_INT);
    catStrWithSpace(temp,posCode);
    //add newline character at the end
    strcat(temp,"\n");

    return temp;
}


/*
FUNCTION: catStrWithSpace
Description: for writing sorting result to fifo, adding a "\n" at the end
*/
void catStrWithSpace(char * dest, char * src){
    strcat(dest,src);
    strcat(dest," ");

}