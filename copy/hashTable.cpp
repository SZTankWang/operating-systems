//
// Created by User on 2021/2/9.
//

#include "hashTable.h"
#include "voter.h"


hashTable::hashTable(int bucketSize) {
    bucketSize = bucketSize;
    table = new voter * [bucketSize];
}
int hashTable::hash(int RIN) {
    return RIN % bucketSize;
}



/*function: insertVoter
 * param: voter v
 * return: status code. 0:success; -1: failure since duplicated
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

        curr = curr->getNextVoter();
        if(curr->getNextVoter() == nullptr){
            //find the avail spot
            curr->setNextVoter(&v);
            return 0;
        }
    }
    //else, current spot is already empty, insert
    curr = &v;
    return 0;



}

int hashTable::deleteVoter() {
    return 0;
}

