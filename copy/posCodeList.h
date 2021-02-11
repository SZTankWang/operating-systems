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
    int posNodeNum;
public:
    posCodeList(posNode* posNodeArg);
    int addNewPosNode(posNode*posNodeArg);
    int voteCountByPlace();

};


#endif //CODING_1_POSCODELIST_H
