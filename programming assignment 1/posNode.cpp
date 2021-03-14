//
// Created by User on 2021/2/11.
//

#include "posNode.h"

#include <iostream>

using namespace std;

/*constructor function
 * description: create a new posNode that maintains three ptr,  voted voter num, an output flag
 * params: int zipcode
 * */

posNode::posNode(int zipCode) {
//    voter arg = voterPtr->getVoter();
    posCode = zipCode;
    votedNum = 0;
    voterHead = nullptr; //ptr to first voted DTO object
    voterTail = nullptr; //ptr to last voted DTO object
    nextPosNode = nullptr;
    output = 0;
}

int posNode::getPosCode() {
    return posCode;
}


/*
 * function:addVoter
 * params: pointer to new voted DTO
 * voted DTO is not voter, it stores a pointer to a voter
 * */
int posNode::addVoter(voted *voterPtr) {
    //if previous list is not empty, insert at votedTail
    if(voterTail!= nullptr){
        voterTail->setNextVoted(voterPtr);
    }
    else{
        voterTail = voterPtr;
        voterHead = voterPtr;
    }
    votedNum ++;
    //error
    return 0;
}

/*
 * function:listVoter
 * Description: list all the voter that voted in the specific zipcode
 * One voter per line
 * */
int posNode::listVoter(){
    voted * tempPtr = voterHead;
    int voterRin;
    while(tempPtr!= nullptr){
        voter arg_voter = tempPtr->getVoter();
        if(arg_voter.getFlag()=='Y'){
            voterRin = arg_voter.getRIN();
            cout<<"RIN:"<<voterRin<<endl;
        }
        tempPtr = tempPtr->getNextVoted();

    }
    return 0;
}

/*
 * function getNextPosNode
 * Description: return pointer-> next postal code node in the linked list
 * */
posNode *posNode::getNextPosNode() {
    return nextPosNode;
}

/*
 * function: getVotedNum
 * return: total number of voted people in this postal code area
 * */
int posNode::getVotedNum() {
    return votedNum;
}

/*
 * function: showVoterList
 * Description: output information of vote. First line: total number of voted people
 * Start from second line: output each voters' RIN
 * */
int posNode::showVoterList() {
    int voterCount;
    voterCount = getVotedNum();
    cout<<"voted number of participants:  "<<voterCount<<endl;
    listVoter();
    return 0;
}

/*
 * function setNextPosNode
 * Description: set up the next node in the linked list.
 * Set the pointer nextPosNode to be posNodeArg
 * */
int posNode::setNextPosNode(posNode *posNodeArg) {
    nextPosNode = posNodeArg;
    return 0;
}


/*function: delete the voted record node that points to a certain voter
 * param: RIN, ID of voter
 * return: 0:success; -1:failure since not exists
 * */
int posNode::deleteVotedRecord(int RIN) {
    //traverse the linked list to find
    voted * curr = voterHead;
    voted * prev = nullptr;
    while(curr!= nullptr){
        voter pointed = curr->getVoter();
        //check the voter it's pointed to, if so, delete
        if(pointed.getRIN()==RIN){
            //if current voted node is the head
            if(prev == nullptr){
                //update linked list head
                voterHead = curr->getNextVoted();
                //if current node is also the tail, which means the linked list is going to be empty
                if(curr == voterTail){
                    voterTail = prev;
                }
                votedNum = votedNum-1;
                free(curr);
                return 0;
            }

            if(curr == voterTail){
                voterTail = prev;
                votedNum = votedNum-1;
                free(curr);
                return 0;
            }

            //else, connect the prev and the next
            voted * next = curr->getNextVoted();
            prev->setNextVoted(next);
            free(curr);
            votedNum = votedNum-1;
            return 0;
        }
        //else, just proceed
        prev = curr;
        curr = curr->getNextVoted();
    }

    return -1;
}


/*function: isOutput
 * Description: used to tell if this posNode has been selected as a local maximum and get output before
 * return: 0: no; 1: yes
 * */
int posNode::isOutput() {
    return output;
}

void posNode::setOutput() {
    output = 1;
}

void posNode::reset() {
    output = 0;
}

