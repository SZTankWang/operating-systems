//
// Created by User on 2021/2/11.
//


#include <iostream>
#include "posCodeList.h"
#include "voter.h"
#include "posNode.h"
using namespace std;

posCodeList::posCodeList() {
    posNodeHead = nullptr;
    posNodeTail = nullptr;
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


/*function: voteCountByPlace
 * not implemented
 * */
int posCodeList::voteCountByPlace() {
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
        curr->getNextPosNode();
    }
    //not found
    return nullptr;

}
