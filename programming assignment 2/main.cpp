#include <iostream>
#include <cstring>

#include <sys/types.h>
#include <unistd.h>

using namespace std;

int main(int argc, char ** argv) {

    // get attributes for the programs
    char * inputFileName;
    int workNum;
    int rangeFlag=1; // 1 for equally sized; 0 for random ranges
    int orderFlag=0; // 0 for ascending; 1 for descending
    int attrNum; // number of attributes
    char * outputFileName;
    for(int i=0;i<argc;i++){
        if(strcmp(argv[i],"-i")==0){
            inputFileName = argv[i+1];
        }
        if(strcmp(argv[i],"-k")==0){
            workNum = atoi(argv[i+1]);
        }
        if(strcmp(argv[i],"-r")==0){
            rangeFlag=0;
        }
        if(strcmp(argv[i],"-a")==0){
           attrNum = atoi(argv[i+1]);
        }
        if(strcmp(argv[i],"-o")==0){
            if(strcmp(argv[i+1],"d")==0){
                orderFlag = 1;
            };
        }
        if(strcmp(argv[i],"-s")==0){
            outputFileName = argv[i+1];
        }


    }
    cout<<"number of worker"<<workNum<<endl;
    cout<<"input file :"<<inputFileName<<endl;
    cout<<"order: "<<orderFlag<<endl;

    //fork the coord node
    if(fork()){}

    return 0;


}
