//
// Created by User on 2021/2/12.
//

#include "voted.h"


/*
 * Object voted
 * Description: used as node of the linked list of each postal code node.
 * Carry information of a voted voter in the postal code area
 * */
voted::voted(voter *ptr) {
    voterPtr = ptr; //ptr to actual voter object
    nextVoted = nullptr; //ptr -> next voted
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


