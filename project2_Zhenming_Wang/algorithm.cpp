#include "Taxpayer.h"
#include "algorithm.h"

#include <cstdlib>
#include <ostream>
#include <iostream>


using namespace std;

/*
FUNCTION swap, used for bubble sort, swapping two elemnt
*/
void swap(Taxpayer * ptr1, Taxpayer * ptr2){
    Taxpayer * temp = (Taxpayer*)malloc(sizeof(Taxpayer));
    *temp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = *temp;
    free(temp);
    // delete temp;
}


//sortAttr: attribute to sort; sortFlag:0:ASCE; 1: DESC; swapped: indicate that a swap has happened
int doSorting(Taxpayer ** payerList,int sortAttr,int sortFlag,int start,int end, int swapped){
    // cout<<"***********INSIDE DO SORTING*******"<<endl;
    if(sortFlag==0){

        switch(sortAttr){

        case 1:
        {

            //sort RID
            if(payerList[start]->getRid()>payerList[end]->getRid()){
                swap(payerList[start],payerList[end]);
                swapped = 1;
            }
            break;

        }


        case 4:

        {

             //sort num of depd
            if(payerList[start]->getNumOfDepd()>payerList[end]->getNumOfDepd()){
                swap(payerList[start],payerList[end]);
                swapped = 1;
            }     

            break;      
        }


        case 5:
        {
            //sort income
            if(payerList[start]->getIncome()>payerList[end]->getIncome()){
                swap(payerList[start],payerList[end]);
                swapped = 1;
            }
            break;
        }


        case 6:
        {

            //sort posCode
            if(payerList[start]->getPosCode()>payerList[end]->getPosCode()){
                swap(payerList[start],payerList[end]);
                swapped = 1;
            }

            break;            
        }

        default:
            cout<<"invalid sorting attribute!"<<endl;
            break;
        }

        return swapped;
    }
    else if(sortFlag==1){

        switch(sortAttr){

        case 1:
        {

             //sort RID
            if(payerList[start]->getRid()<payerList[end]->getRid()){
                swap(payerList[start],payerList[end]);
                swapped = 1;
            }      
            break;     
        }


        case 4:
        {
             //sort num of depd
            if(payerList[start]->getNumOfDepd()<payerList[end]->getNumOfDepd()){
                swap(payerList[start],payerList[end]);
                swapped = 1;
            }   
            break;         
        }


        case 5:
        {
             //sort income
            if(payerList[start]->getIncome()<payerList[end]->getIncome()){
                swap(payerList[start],payerList[end]);
                swapped = 1;
            }   
            break;         
        }


        case 6:
        {
             //sort posCode
            if(payerList[start]->getPosCode()<payerList[end]->getPosCode()){
                swap(payerList[start],payerList[end]);
                swapped = 1;
            }       
            break;     
        }


        default:
            cout<<"invalid sorting attribute!"<<endl;
            break; 
        }

        return swapped;
    }

    else{
        cout<<"invalid sort flag"<<endl; //invalid sortFlag
        return swapped;
    }


}
/*
FUNCTION for bubble sort
*/
//flag=0: sort in ASCE; flag=1: DESC
void BubbleSort(Taxpayer ** payerList,int length,int flag,int sortAttr){
	int swapped = 0;
    int start;
    int end;
    // cout<<"*********INSIDE BUBBLE********"<<endl;
	while(1){
		for(int i=0;i<length-1;i++){
            start = i;
            end = i+1;
            swapped = doSorting(payerList,sortAttr,flag,start,end,swapped);

		}
		if(swapped==0){
			// cout<<"*************SWAPPED IS ZERO**********"<<endl;
            break;

		}
		else{
			swapped = 0;
		}

	}
}

/*
function for insertion sort.
flag: 0 - ASEC; 1 - DESC
*/
void insertionSort(Taxpayer ** payerList, int length, int flag, int sortAttr){
    int i,j;
    Taxpayer * key;
    for(i=1;i<length;i++){
        key = payerList[i];
        j=i-1;

        if(flag == 0){
                //ASEC
            switch(sortAttr){

            case 1:
            {

                 //sort RID
                while(j>=0 && payerList[j]->getRid()>key->getRid()){
                    payerList[j+1] = payerList[j];
                    j = j-1;

                }
                payerList[j+1] = key;
                break;     
            }


            case 4:
            {
                 //sort num of depd
                while(j>=0 && payerList[j]->getNumOfDepd()>key->getNumOfDepd()){
                    payerList[j+1] = payerList[j];
                    j = j-1;

                }
                payerList[j+1] = key;
                break;         
            }


            case 5:
            {
                 //sort income
                while(j>=0 && payerList[j]->getIncome()>key->getIncome()){
                    payerList[j+1] = payerList[j];
                    j = j-1;

                }
                payerList[j+1] = key;
                break;         
            }


            case 6:
            {
                 //sort posCode
                while(j>=0 && payerList[j]->getPosCode()>key->getRid()){
                    payerList[j+1] = payerList[j];
                    j = j-1;

                }
                payerList[j+1] = key;     
                break;     
            }


            default:
                cout<<"invalid sorting attribute!"<<endl;
                break; 
            }

        }
         else if(flag == 1){
        //SORT DESC
            switch(sortAttr){

            case 1:
            {

                 //sort RID
                while(j>=0 && payerList[j]->getRid()<key->getRid()){
                    payerList[j+1] = payerList[j];
                    j = j-1;

                }
                payerList[j+1] = key;
                break;     
            }


            case 4:
            {
                 //sort num of depd
                while(j>=0 && payerList[j]->getNumOfDepd()<key->getNumOfDepd()){
                    payerList[j+1] = payerList[j];
                    j = j-1;

                }
                payerList[j+1] = key;
                break;         
            }


            case 5:
            {
                 //sort income
                while(j>=0 && payerList[j]->getIncome()<key->getIncome()){
                    payerList[j+1] = payerList[j];
                    j = j-1;

                }
                payerList[j+1] = key;
                break;         
            }


            case 6:
            {
                 //sort posCode
                while(j>=0 && payerList[j]->getPosCode()<key->getRid()){
                    payerList[j+1] = payerList[j];
                    j = j-1;

                }
                payerList[j+1] = key;     
                break;     
            }


            default:
                cout<<"invalid sorting attribute!"<<endl;
                break; 
            }

        }

    }
}
