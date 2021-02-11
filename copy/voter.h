//
// Created by User on 2021/2/2.
//

#ifndef CODING_1_VOTER_H
#define CODING_1_VOTER_H


class voter {
private:
    char firstName;
    char lastName;
    int RIN;
    int posCode;
    char flag;
    voter * next;
public:
    voter(char firstName,char lastName, int RIN, int posCode);
    void setFirstName(char newName);
    void setLastName(char newName);
    void setRIN(int RIN);
    void setPosCode(int newPosCode);
    void setFlag(char newFlag);

    char getFirstName();
    char getLastName();
    int getRIN();
    int getPosCode();
    char getFlag();
    int setNextVoter(voter * next);
    voter* getNextVoter();
};


#endif //CODING_1_VOTER_H
