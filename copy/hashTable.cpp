//
// Created by User on 2021/2/9.
//

#include <cstdlib>
#include <iostream>
#include "hashTable.h"
#include "voter.h"
#include "info.h"
#include "posCodeList.h"
#include "voted.h"

using namespace std;

hashTable::hashTable(int bucketSize) {
    bucketSize = bucketSize;
    table = new voter * [bucketSize];
    //initialize the table with nullptr of bucketSize number
    for(int i=0;i<bucketSize;i++){
        table[i] = nullptr;
    }

    //initialize posList pointer
    posCodeList * posList = new posCodeList();
}
int hashTable::hash(int RIN) {
    return RIN % bucketSize;
}



/*function: insertVoter
 * param: voter v
 * return: status code. 0:success; -1: failure for being duplicated
 * */
int hashTable::insertVoter(voter v) {
    int h = hash(v.getRIN());
    //first check if the voter is already there
    //start from table[h]
    voter * curr = table[h];
    while(curr != nullptr &&  curr->getRIN() != v.getRIN()){

        //first check if duplicate
        if(curr->getRIN()==v.getRIN()){
            return -1;
        }


        if(curr->getNextVoter() == nullptr){
            //find the avail spot
            curr->setNextVoter(&v);

            //add this pointer to the posNode voted list
            voterNum ++;
            return 0;
        }
        //else, proceed
        curr = curr->getNextVoter();
    }
    //else, current spot is already empty, insert
    curr = &v;
    voterNum++;
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
            prev->setNextVoter(next);
            free(curr);
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
 * return: 0:success; -1:already voted;-2:not exists
 * */
int hashTable::doVote(int keys[],int size) {
    int i = 0;
    //do vote for every voter with specific RIN
    for(i;i<size;i++){
        info info = lookUpVoter(keys[i]);
        int RIN = keys[i];
        //do vote depending on the look up result
        if(info.getFlag()=='Y'){

            //voted, output message
            cout<<"voter"<<RIN<<": this voter already voted!";
        }
        if(info.getPtr()== nullptr){
            //not exists
            cout<<"voter"<<RIN<<": voter does not exists!";
        }
        if(info.getFlag()=='N'){
            voter * ptr = info.getPtr();
            ptr ->setFlag('Y');

            //update the posCodeList
            //first traverse to find the posCode Node
            int posCode = ptr->getPosCode();
            posNode * nodePtr;
            nodePtr = posList ->findNode(posCode);

            //already exists a node for this postal code
            if(nodePtr!= nullptr){
                //create a voted object to point to this voter
                voted newVoted = voted(ptr);
                nodePtr->addVoter(&newVoted);
            }
            //does not exist such as node, create one
            else if(nodePtr== nullptr){
                voted newVoted = voted(ptr);
                posNode newPosNode = posNode(&newVoted);
                posList->addNewPosNode(&newPosNode);
            }
            cout<<"voter"<<RIN<<"successfully voted!";
        }
    }
    return 0;
}


/*function: lookUpVoter
 * Description: look up the hashtable to find a voter
 * param: RIN voter ID
 * return: pointer to actual node if exists/ nullptr if not
 * */
info hashTable::lookUpVoter(int RIN) {
    int KEY = hash(RIN);
    voter * temp = table[KEY];

    while(temp != nullptr){
        if(temp->getRIN()==RIN && temp->getFlag()=='N'){
            info info('N',temp);
            return info;
        }
        if(temp->getFlag()=='Y'){
            info info('Y',temp);
            return info;
        }
        temp = temp->getNextVoter();
    }

    //not found
    info info(' ', nullptr);
    return info;
}

