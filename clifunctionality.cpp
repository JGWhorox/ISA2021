/**
 * author Johann A. Gawron - xgawro00
 * file argparser.h
 * brief implementation of various CLI functionalities as defined by assignment
 */


#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <cstdio>

#include "clifunctionality.h"

using namespace std;

std::vector<string> split(string input){
    vector<string> retvals;
    istringstream strStream(input);
    for(string s; strStream>>s;){
        retvals.push_back(s);
    }
    return retvals;
}

void printDebug(Arguments &args){
    cout << "############# DEBUG INFO #############" << endl;
    cout << "write?: \t" << args.write << endl;
    cout << "file path:\t " << args.filePath << endl;
    cout << "timeout:\t" << args.timeout << endl;
    cout << "max size:\t" << args.blockSize << endl;
    cout << "multicast?:\t" << args.multicast << endl;
    cout << "bin mode?:\t" << args.binaryMode << endl;
    cout << "address:\t" << args.address << endl;
}

void printHelp(){
    printf("help \n future help here\n");
}

bool parseArgs(Arguments &args, string input){
    if(input.empty()){
        return false;
    }

    vector<string> splitInput = split(input);

    bool foundRW = false;

    for(auto it = splitInput.begin(); it != splitInput.end(); ++it){
        //read?
        if((*it == "-R") && (!foundRW)){
            foundRW = true;
            args.write = false;
        }
        //write?
        else if((*it == "-W") && (!foundRW)){
            foundRW = true;
            args.write = true;
        }
        //filepath
        else if(*it == "-d"){
            if(it+1 != splitInput.end()){
                args.filePath = *(++it);
            }
        }
        //timeout
        else if(*it == "-t"){
            if(it+1 != splitInput.end()){
                try{
                    int tryIfInt = stoi(*(++it));
                    args.timeout = tryIfInt;
                }
                catch (const std::exception&){
                    cout << "timeout needs to be given as integer" << endl;
                    return false;
                }             
            }
        }
        //blocksize
        else if(*it == "-s"){
            if(it+1 != splitInput.end()){
                try{
                    int tryIfInt = stoi(*(++it));
                    args.blockSize = tryIfInt;
                }
                catch (const std::exception&){
                    cout << "blocksize needs to be given as integer" << endl;
                    return false;
                }             
            }
        }
        //multicast?
        else if(*it == "-m"){
            args.multicast = true;
        }
        //mode
        else if(*it == "-c"){
            if(it+1 != splitInput.end()){
                auto mode = *(++it);
                if((mode == "ascii") || (mode == "netascii")){
                    args.binaryMode = false;
                }
            }
        }
        //address
        else if(*it == "-a"){
            if(it+1 != splitInput.end()){
                auto address = *(++it);
                string ip = address.substr(0,address.find(","));
                string port = address.substr(address.find(",")+1,address.length());
                cout << ip << "< toz ip a toz port >" << port << endl;
            }
        }
        else{
            return false;
        }


        //printDebug(args);
    }
    
    
    if(!foundRW){
        return false;
    }

    return true;
}

