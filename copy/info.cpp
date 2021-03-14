//
// Created by User on 2021/2/12.
//

#include "info.h"

info::info(char flagArg, voter *ptrArg) {
    flag = flagArg;
    ptr = ptrArg;
}


char info::getFlag() {
    return flag;
}

voter* info::getPtr() {
    return ptr;
}