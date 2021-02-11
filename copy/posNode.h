//
// Created by User on 2021/2/11.
//

#ifndef CODING_1_POSNODE_H
#define CODING_1_POSNODE_H

#include "voter.h"

class posNode {
private:
    int * posCode;
    voter * voterHead;
    posNode * nextPosNode;
public:
    posNode(voter*voterPtr);
    int getPosCode();
    int listVoter();
    int addVoter(voter * voterPtr);
    posNode * getNextPosNode();
    int setNextPosNode(posNode*posNodeArg);
    int getVotedNum();
    int showVoterList();
};



#endif //CODING_1_POSNODE_H
