//
// Created by User on 2021/2/11.
//


#include <iostream>
#include "posCodeList.h"
#include "voter.h"

using namespace std;

posCodeList::posCodeList(posNode * posNodeArg) {
    posNodeHead = nullptr;
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
            return 0;
        }
        tempPtr = tempPtr->getNextPosNode();

    }

    tempPtr = posNodeArg;
    return 0;
}


/*function: voteCountByPlace
 * not implemented
 * */
int posCodeList::voteCountByPlace() {
    return 0;
}