//
// Created by User on 2021/2/12.
//

#include "info.h"

/*
 * DTO info
 * Description: used when querying whether a voter exists / have voted already
 * */
info::info(char flagArg, voter *ptrArg) {
    flag = flagArg; //vote flag. 'Y': Voted; 'N': Not Voted
    ptr = ptrArg; //pointer to the voter
}


char info::getFlag() {
    return flag;
}

voter* info::getPtr() {
    return ptr;
}