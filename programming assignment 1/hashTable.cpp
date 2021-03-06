//
// Created by User on 2021/2/9.
//

#include <cstdlib>
#include <iostream>
#include "hashTable.h"


using namespace std;

/*
 * function: constructor
 * params: int bucketSize: number of buckets
 * Description: automatically initialize an array of pointers to voter objects, as well
 * as a new linked list of postal code. The hash table also maintain a counter of num of voters
 * */
hashTable::hashTable(int bucketSizeArg) {
    bucketSize = bucketSizeArg;
    table = new  voter*[bucketSize] ;

    //initialize posList pointer
    posList = new posCodeList();
    voterNum=0;
}

/*
 * function: hash
 * params: RIN of a voter
 * return: the hash value of key RIN
 *
 * */
int hashTable::hash(int RIN) {
    int result = RIN % bucketSize;
    return result;
}



/*function: insertVoter
 * param: voter v
 * Description: on inserting new voter, also determining if we need a new postal code Node/
 * return: status code. 0:success; -1: failure for being duplicated
 * */
int hashTable::insertVoter(voter * v) {
    int h = hash(v->getRIN());
    int zip = v->getPosCode();
    posNode * nodePtr;
    //first check if the voter is already there
    //start from table[h]


    voter * curr = table[h];
    while(curr != nullptr){

        //first check if duplicate
        if(curr->getRIN()==v->getRIN()){
            return -1;
        }


        if(curr->getNextVoter() == nullptr){
            //find the avail spot
            curr->setNextVoter(v);

            //add this pointer to the posNode voted list
            voterNum ++;

            //check if we need to create a new posCode node
            nodePtr = posList->findNode(zip);
            if(nodePtr== nullptr){
                //new posCode, create one node
                posNode * newNode = new posNode(zip);
                posList->addNewPosNode(newNode);
            }

            return 0;
        }
        //else, proceed
        curr = curr->getNextVoter();
    }
    //else, current spot is already empty, insert
    table[h] = v;
    voterNum++;

    //check if we need to create a new posCode node
    nodePtr = posList->findNode(zip);
    if(nodePtr== nullptr){
        //new posCode, create one node
        posNode * newNode = new posNode(zip);
        posList->addNewPosNode(newNode);
    }
    return 0;

}



/*function: deleteVoter
 * Description: delete a voter in the hashtable with certain RIN
 * param: Voter ID RIN
 * return : 0:success; -1:failure
 * */
int hashTable::deleteVoter(int RIN) {
    //find this voter
    int KEY = hash(RIN);
    voter * curr = table[KEY];
    voter * prev = nullptr;
    while(curr!= nullptr){
        //check voter, if RIN matches, delete
        if(curr->getRIN()==RIN){
            voter * next = curr->getNextVoter();


            //if current is the head of the linked list
            if(prev == nullptr){
                table[KEY] = next;
                delete(curr);
                --voterNum;
                return 0;
            }
            //else, normal case
            prev->setNextVoter(next);
//            cout<<"debug info"<<endl;
//            int rin = curr->getRIN();
//            cout<<rin<<endl;
            delete(curr);
            --voterNum;
            return 0;
        }
        //else, proceed
        prev = curr;
        curr = curr->getNextVoter();
    }

    //not found, return -1
    return -1;
}


/*function: doVote
 * description: voting for a group of voters
 * params: keys[], array of RINs
 * return: 0:success; -1:already voted;-2:not exists; -3: unexpected
 * */
int hashTable::doVote(int key) {

    //do vote for a voter with specific RIN

    info info = lookUpVoter(key);
    int RIN = key;
    //do vote depending on the look up result
    if(info.getFlag()=='Y'){

        //voted, output message
        cout<<"voter "<<RIN<<": this voter already voted!"<<endl;
        return -1;
    }
    else if(info.getPtr()== nullptr){
        //not exists
        cout<<"voter "<<RIN<<": voter does not exists!"<<endl;
        return -2;
    }
    else if(info.getFlag()=='N'){
        voter * ptr = info.getPtr();
        ptr ->setFlag('Y');

        //update the posCodeList
        //first traverse to find the posCode Node
        int posCode = ptr->getPosCode();
        posNode * nodePtr;
        nodePtr = posList ->findNode(posCode);
        voted * newVoted = new voted(ptr);
        //already exists a node for this postal code
        if(nodePtr!= nullptr){
            //create a voted object to point to this voter
            nodePtr->addVoter(newVoted);
        }
//        //does not exist such as node, create one(deprecated, now all the postal ode node
//        should be initialized on inserting voter)
//        else if(nodePtr== nullptr){
//
//            posNode * newPosNode = new posNode(newVoted);
//            posList->addNewPosNode(newPosNode);
//        }
        cout<<"voter "<<RIN<<" successfully voted!"<<endl;
        return 0;
    }
    cout<<"unexpected error"<<endl;
    return -3;
}


/*function: lookUpVoter
 * Description: look up the hashtable to find a voter
 * param: RIN voter ID
 * return: Info DTO. carry pointer to actual node and vote status if exists/ nullptr if not
 * */
info hashTable::lookUpVoter(int RIN) {
    int KEY = hash(RIN);

    voter * temp = table[KEY];

    while(temp != nullptr){
        if(temp->getRIN()==RIN){
            char flag = temp->getFlag();
//            char * namePtr = temp->getFirstName();

            info returnInfo = info(flag,temp);
            return returnInfo;
        }

        temp = temp->getNextVoter();
    }

    //not found
    info returnInfo(' ', nullptr);
    return returnInfo;
}

/*
 * function getSize
 * return the number of voters
 * */
int hashTable::getSize() {
    return voterNum;
}

/*
 * function: getTablePtr
 * Description: return the pointer to pointers of voters
 * return: **voter
 * */
voter **hashTable::getTablePtr() {
    return table;
}

/*
 * function: getTotalVote
 * Description: get the number of people that have voted, which could be found out by traversing
 * the postal code linked list
 * return : int result
 * */
int hashTable::getTotalVote() {

    int result = posList->getVotedTotalCount();
    return result;
}

/*
 * function:getListPointer
 * Description: return pointer to the postal code linked list
 * return: *posCodeList
 * */
posCodeList *hashTable::getListPointer() {
    return posList;
}

