                while(read_flag == 1){
                    // cout<<"merger is going to listen to fifo from here ..."<<endl;
                    read_flag=0;
                    FD_ZERO(&fds);
                    setFdToSet(pipeList,&fds);
                    // cout<<"max fd is"<<maxfd<<endl;

                    select(maxfd+1,&fds,NULL,NULL,NULL);
                    // cout<<"hello??"<<endl;

                    //try to read from possible fifos
                    int a=0;
                    while(a < workerNum){
                        myfifo * temp_ptr = pipeList[a];
                        int temp_fd = temp_ptr->fd;
                        if(FD_ISSET(temp_fd,&fds) != 0){
                            
                            //we can read from this fifo
                            //if we already have a buffer for this fifo, continue to use                            

                            //CHECK IF THIS BUFFER HAS BEEN READ BEFORE
                            char * readBuffer = NULL;
                            int bufferSize = 0;
                            if(bufferList[a] != NULL){
                                readBuffer = bufferList[a];
                                //check size and read byte
                                bufferSize = buffer_size_list[2*a];
                                int used = buffer_size_list[2*a + 1];
                                if(used >= bufferSize/2){
                                    //realloc
                                    char * newBuffer = (char*)realloc(readBuffer,bufferSize*2);
                                    if(newBuffer==NULL){
                                        cout<<"realloc error on line 174"<<endl;
                                        break;
                                    }
                                    readBuffer = newBuffer;
                                    buffer_size_list[2*a] = bufferSize * 2;
                                }

                            }
                            else{
                                bufferSize = 2048;
                                readBuffer = (char *)malloc(sizeof(char)*bufferSize);
                                bufferList[a] = readBuffer;//record the buffer address into the list
                                buffer_size_list[2*a+1] = 0;
                                buffer_size_list[2*a] = bufferSize;
                            }


                            ssize_t total_read = buffer_size_list[2*a+1];
                            ssize_t nread;

                            // char * bufferPtr = readBuffer;
                            cout<<"begin to read fifo"<<temp_ptr->pathname<<endl; 

                            //start to read from now  
                            while(nread > 0){
                                read_flag = 1;
                                total_read = total_read + nread;
                                cout<<"total read is"<<total_read<<endl;
                                cout<<readBuffer<<endl;
                                
                                buffer_size_list[2*a + 1]=total_read;//update used byte in buffer_size_list
                                    
                                //realloc buffer if needed
                                if(nread >= bufferSize/2 ){
                                  char * temp = (char*)realloc(readBuffer,bufferSize*2);
                                  if(temp == NULL){
                                    cout<<"reallocation fail!"<<endl;
                                    exit(-1);

                                  } 

                                  else{
                                    bufferSize *= 2;
                                    buffer_size_list[2*a] = bufferSize;

                                    readBuffer = temp;
                                    bufferList[a] = temp;

                                  } 
                                }
                                nread = read(temp_fd,readBuffer,bufferSize/2);        
                            }                              

                            if(bufferList[a]==NULL){
                                cout<<"line 227: buffer is null"<<endl;
                            }
                            cout<<"CONTENT ON BUFFER"<<a<<" "<<bufferList[a]<<endl;


                            //close pipe
                            close(temp_fd);

                            

                        };

                        a++;
                    }

                }