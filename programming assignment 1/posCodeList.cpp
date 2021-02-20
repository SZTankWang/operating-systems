//
// Created by User on 2021/2/11.
//


#include <iostream>
#include "posCodeList.h"

using namespace std;

posCodeList::posCodeList() {
    posNodeHead = nullptr; //head of postal code list
    posNodeTail = nullptr; // tail of postal code list
    posNodeNum = 0;
}


/*function: addNewPosCode
 * description: insert new posNode into the linked list if not exists.
 * return : -1: failure; 0:success
 * */
int posCodeList::addNewPosNode(posNode *posNodeArg) {
    posNode * tempPtr;
    tempPtr = posNodeHead;
    //check if the posCodeNode already exists
    int posCode = posNodeArg->getPosCode();
    while(tempPtr!= nullptr){
        if(tempPtr->getPosCode() == posCode){
            return -1;
        }

        if(tempPtr->getNextPosNode()== nullptr){
            tempPtr->setNextPosNode(posNodeArg);
            posNodeTail = posNodeArg;
            posNodeNum++;
            return 0;
        }
        tempPtr = tempPtr->getNextPosNode();

    }

    //list is empty, then add in the first node
    posNodeHead=posNodeArg;
    posNodeTail=posNodeArg;
    posNodeNum++;
    return 0;
}

/*
 * function: addNewPosCode (NEW)
 * Description: directly insert new postal ode node at the end of current linked list
 * if current linked list is empty, set both head and tail to be posNodeArg
 *
 * */

int posCodeList::addNewPosNodeAlt(posNode *posNodeArg){
    if(posNodeTail!= nullptr){
        posNodeTail->setNextPosNode(posNodeArg);
    }
    else{
        posNodeHead = posNodeArg;
        posNodeTail = posNodeArg;
    }
    posNodeNum++;
    return 0;
}



/*function: findNode
 * Description: find out if a specific postal code node already exists
 * params: int zip: postal code
 * return : exists: pointer to that node; no: return nullptr
 * */
posNode *posCodeList::findNode(int zip) {
    posNode * curr = posNodeHead;
    //if posCodeList is currently empty: (this would be handled in the outer logic, not here)


    while(curr!= nullptr){
        if(curr->getPosCode()==zip){
            //found
            return curr;
        }
        curr = curr->getNextPosNode();
    }
    //not found
    return nullptr;

}


/*function: getVotedTotalCount
 * Description: return total number of participants who already voted
 * return: total number
 * */
int posCodeList::getVotedTotalCount() {
    posNode * temp = posNodeHead;
    int total = 0;

    while(temp != nullptr){
        total += temp->getVotedNum();
        temp = temp->getNextPosNode();
    }

    return total;
}

/*
 * function: isEmpty
 * Description: if current postal code list has no node, return 0; else, return -1
 * */

int posCodeList::isEmpty() {
    if(posNodeNum!=0){
        return -1;
    }
    else{
        return 0;
    }
}


/*function: outputDesc
 * Description: output the number of voted people for each zipcode in DESC
 * output: zipcode | number of votes
 * */
void posCodeList::outputDesc() {
    posNode * temp = posNodeHead;

    posNode * memo;
    int size = posNodeNum;
    for(int a=0;a<size;a++){
        int max = 0;
        while(temp != nullptr){

            int flag = temp->isOutput();
            //traverse to find local maximum
            if(flag==0){
                int local = temp->getVotedNum();


                if(local>=max ){
                    max = local;
                    memo = temp;
                }
            }

            temp = temp->getNextPosNode();

        }
        //set up the output flag of the local maximum node
        memo->setOutput();
        int zip = memo->getPosCode();
        int vote = memo->getVotedNum();
        cout<<"ZIP: "<<zip<<" | "<<"voted people: "<<vote<<endl;
        //restore temp pointer for the next loop
        temp = posNodeHead;

    }

    //finished, reset all node's output flag to 1. Important.
    while(temp != nullptr){
        temp->reset();
        temp = temp->getNextPosNode();
    }
}
