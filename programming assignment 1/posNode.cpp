//
// Created by User on 2021/2/11.
//

#include "posNode.h"

#include <iostream>
#include "voter.h"
#include "voted.h"
#include "posNode.h"
using namespace std;

/*constructor function
 * description: create a new posNode with posCode ptr, voted voter num
 * params: pointer to a voted object
 * */

posNode::posNode(voted *voterPtr) {
    voter arg = voterPtr->getVoter();
    posCode = arg.getPosCode();
    votedNum = 1;
    voterHead = voterPtr;
    voterTail = voterPtr;
    nextPosNode = nullptr;
    output = 0;
}

int posNode::getPosCode() {
    return posCode;
}

int posNode::addVoter(voted *voterPtr) {
    //insert at votedTail
    voterTail->setNextVoted(voterPtr);
    votedNum ++;
    //error
    return 0;
}

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

posNode *posNode::getNextPosNode() {
    return nextPosNode;
}

int posNode::getVotedNum() {
    return votedNum;
}

int posNode::showVoterList() {
    int voterCount;
    voterCount = getVotedNum();
    cout<<"voted number of participants:  "<<voterCount<<endl;
    listVoter();
    return 0;
}

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

