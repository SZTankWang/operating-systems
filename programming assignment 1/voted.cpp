//
// Created by User on 2021/2/12.
//

#include "voted.h"

voted::voted(voter *ptr) {
    voterPtr = ptr;
    nextVoted = nullptr;
}

voter voted::getVoter() {
    return *voterPtr;

}

voted * voted::getNextVoted() {
    return nextVoted;
}

int voted::setNextVoted(voted *ptr) {
    nextVoted = ptr;
    return 0;
}


