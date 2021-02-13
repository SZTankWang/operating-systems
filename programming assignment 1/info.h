//
// Created by User on 2021/2/12.
//

#ifndef CODING_1_INFO_H
#define CODING_1_INFO_H
#include "voter.h"


/*info class
 * description: carry the information of a certain voter node
 * params: voter * voterPtr
 * */
class info {
private:
    char flag;
    voter* ptr;

public:
    info(char flagArg,voter * ptrArg);
    void setFlag(char flagArg);
    void setVoter(voter * ptrArg);
    char getFlag();
    voter* getPtr();
};


#endif //CODING_1_INFO_H
