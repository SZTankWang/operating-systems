#include <iostream>
#include <string.h>

using namespace std;

int main( int argc, char **argv ) {
    cout << "starting mvote..." << endl;
    int run_flag = 1;
    char * actionBuffer;
    char * action;
    char * readPtr;
    actionBuffer = (char*)malloc(100*sizeof(char));
    action = (char*)malloc(16*sizeof(char));

    //parse command line argument
    cout<<"file to read:"<<argv[2];
    cout<<"\n"<<"hashtable size:"<<argv[4];

    while(run_flag == 1){
        cout<<"what do you want? "<<endl;
        cin.getline(actionBuffer,1024,'-');
        //Look for the space in the input
        readPtr = strchr(actionBuffer,'\0');

        if(readPtr != NULL){
            printf("found at %d\n",readPtr-actionBuffer+1);
            strncpy(action,actionBuffer,readPtr-actionBuffer);
            if(strcmp(action,"exit")==0){
                cout<<"the program stop here";
                run_flag = 0;
                free(actionBuffer);
                free(action);
            };
        };

        // depend on the operation specified before the first empty space

    }
    free(actionBuffer);
    free(action);
    return 0;

}
