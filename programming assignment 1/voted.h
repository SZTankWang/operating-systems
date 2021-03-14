//
// Created by User on 2021/2/12.
//

#ifndef CODING_1_VOTED_H
#define CODING_1_VOTED_H
#include "voter.h"

class voted {
private:
    voter * voterPtr;
    voted * nextVoted;

public:
    voted(voter * ptr);
    voter getVoter();
    voted * getNextVoted();
    int setNextVoted(voted * ptr);
};


#endif //CODING_1_VOTED_H
