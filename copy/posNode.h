//
// Created by User on 2021/2/11.
//

#ifndef CODING_1_POSNODE_H
#define CODING_1_POSNODE_H

#include "voter.h"
#include "voted.h"
class posNode {
private:
    int * posCode;
    int votedNum;
    voted * voterHead;
    voted * voterTail;
    posNode * nextPosNode;

public:
    posNode(voted*voterPtr);
    int getPosCode();
    int listVoter();
    int addVoter(voted * voterPtr);
    posNode * getNextPosNode();
    int setNextPosNode(posNode*posNodeArg);
    int getVotedNum();
    int showVoterList();

    int deleteVotedRecord(int RIN);
};



#endif //CODING_1_POSNODE_H
