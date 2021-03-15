#include <iostream>
#include <string.h>

using namespace std;

int main( int argc, char **argv ) {
    cout << "starting mvote..." << endl;
    int run_flag = 1;
    char * actionBuffer;
    char * action;
    char * readPtr;
    char exit[4] = {'e','x','i','t'};

    //parse command line argument
    char * filename;
    filename = argv[3];
    cout<<"filename: "<<*filename<<"\n";

    while(run_flag == 1){
        actionBuffer = (char*)malloc(100*sizeof(char));
        action = (char*)malloc(16*sizeof(char));

        cout<<"what do you want?\n "<<endl;
        cin.getline(actionBuffer,1024);

        if(strcmp(actionBuffer, exit)==0){
            cout<<"bye!";
            free(actionBuffer);
            free(action);
            run_flag=0;
            break;
        }
        //if not exit, Look for the space in the input
        readPtr = strchr(actionBuffer,' ');


        int size = readPtr-actionBuffer;
        //extract the operation

        strncpy(action,actionBuffer,size);
        for(int i=0;i<size;i++){
            cout<<"operation:"<<action[i];
        }


        // depend on the operation specified before the first empty space
        free(actionBuffer);
        free(action);
        run_flag =0;
    }
    free(actionBuffer);
    free(action);
    return 0;

}
