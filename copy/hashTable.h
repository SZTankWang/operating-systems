//
// Created by User on 2021/2/9.
//

#ifndef CODING_1_HASHTABLE_H
#define CODING_1_HASHTABLE_H

#include "voter.h"

class hashTable {
private:

    int bucketSize;
    voter ** table;
public:
    hashTable(int bucketSize);

    int hash(int RIN);
    int insertVoter(voter v);
    int deleteVoter();
    voter lookUpVoter(int RIN);
    int doVote(int keys[]);

};


#endif //CODING_1_HASHTABLE_H
