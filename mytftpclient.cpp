/**
 * author Johann A. Gawron - xgawro00
 * file mytftpclient.cpp
 * brief main program code
 */


#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <sstream>
#include <arpa/inet.h>
#include <sys/socket.h>

//custom
#include "pcapDataTypes.h"
#include "mytftpclient.h"
#include "clifunctionality.h"
#include "tftpSocket.h"


using namespace std;


int main(int argc, char** argv){
    
    if(argc > 1){
        printHelp();
    }
    Arguments args;
    
    //main CLI interface loop 
    while(1){
        
        string input;
        getline(cin, input);
        
        if(parseArgs(args, input)){
            //write 
            if(args.write){ 
                writeToServer(args);
            }
            //read
            else{
                if(args.ipv4){
                    readFromServer(args);
                }
                else{
                    readFromServerInIPv6(args);
                }                  
            }
            
        }
        else{
            cerr << "Given arguments are incorrect!!!" << endl;
            printHelp();
        }
    }
    return 0;
}