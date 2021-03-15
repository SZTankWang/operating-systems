#include "Taxpayer.h"
#include <cstddef>
#include <cstdlib>

Taxpayer::Taxpayer(){

};

Taxpayer::~Taxpayer(){

    if(firstName != NULL){
        free(firstName);
    }
    if(lastName != NULL){
        free(lastName);
    }

}

int Taxpayer::getRid(){
    return this->RID;
}

void Taxpayer::setRid(int RID){
    this->RID = RID;
}

double Taxpayer::getIncome(){
    return this->income;
}

void Taxpayer::setIncome(double income){
    this->income = income;
}

char * Taxpayer::getFirstName(){
    return this->firstName;
}

void Taxpayer::setFirstName(char * firstName){
    this->firstName = firstName;

}

char * Taxpayer::getLastName(){
    return this->lastName;
}

void Taxpayer::setLastName(char * lastName){
    this->lastName = lastName;

}

int Taxpayer::getPosCode(){
    return this->posCode;
}

void Taxpayer::setPosCode(int posCode){
    this->posCode = posCode;
}

int Taxpayer::getNumOfDepd(){
    return this->numOfDepd;
}

void Taxpayer::setNumOfDepd(int numOfDepd){
    this->numOfDepd= numOfDepd;
}
