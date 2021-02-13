//
// Created by User on 2021/2/12.
//

#ifndef CODING_1_HELPER_H
#define CODING_1_HELPER_H


class helper {
public:
    char* parseCommand(char * bufferPtr);
    char * findParam(char * bufferPtr);


    int translateKey(char * paramPtrArg,char * readPtrArg,char * keyArr, int paramIndex);
};


#endif //CODING_1_HELPER_H
