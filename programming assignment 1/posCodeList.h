//
// Created by User on 2021/2/11.
//

#ifndef CODING_1_POSCODELIST_H
#define CODING_1_POSCODELIST_H

#include "posNode.h"
#include "voter.h"

/*class posCodeList
 * description: a linked list, keep reference of posNodes
 * */
class posCodeList {
private:
    posNode * posNodeHead;
    posNode * posNodeTail;
    int posNodeNum;
public:
    posCodeList();
    int addNewPosNode(posNode*posNodeArg);
    int voteCountByPlace();

    posNode * findNode(int zip);

    int getVotedTotalCount();

    int isEmpty();


    void outputDesc();

    //alternative for addNewPosNode, does not start from head
    int addNewPosNodeAlt(posNode *posNodeArg);
};


#endif //CODING_1_POSCODELIST_H
