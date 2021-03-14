//
// Created by User on 2021/2/9.
//

#ifndef CODING_1_HASHTABLE_H
#define CODING_1_HASHTABLE_H

#include "voter.h"
#include "info.h"
#include "posCodeList.h"


class hashTable {
private:

    int bucketSize;
    int voterNum;
    voter ** table;
    posCodeList * posList;
public:
    hashTable(int bucketSize);

    int hash(int RIN);

    int insertVoter(voter v);

    int deleteVoter(int RIN);

    info lookUpVoter(int RIN);

    int doVote(int keys[],int size);


};


#endif //CODING_1_HASHTABLE_H
