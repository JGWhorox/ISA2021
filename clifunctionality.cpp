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
#include <bits/stdc++.h>
#include <arpa/inet.h>

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
    cout << "############## DEBUG INFO ##############" << endl;
    cout << "write?: \t" << args.write << endl;
    cout << "file path:\t " << args.filePath << endl;
    cout << "timeout:\t" << args.timeout << endl;
    cout << "max size:\t" << args.blockSize << endl;
    cout << "multicast?:\t" << args.multicast << endl;
    cout << "bin mode?:\t" << args.binaryMode << endl;
    cout << "address:\t" << args.address << endl;
    cout << "port:   \t" << args.port << endl;
    cout << "############# END OF DEBUG #############" << endl;
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
            
                string port = address.substr(address.find(",")+1,address.length());
                address = address.substr(0,address.find(","));

                regex regex_ipv4("(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");
                regex regex_ipv6("((([0-9a-fA-F]){1,4})\\:){7}([0-9a-fA-F]){1,4}");
                regex regex_port("^([0-9]{1,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$");
                if((!regex_match(address, regex_ipv4)) && (!regex_match(address, regex_ipv6))){
                    cout << "Invalid IP address!\n";
                    return false;
                }
                if(!regex_match(port, regex_port)){
                    cout << "Invalid port number!\n";
                    return false;
                }

                args.address = address;
                args.port = port;
            }
        }
        else{
            return false;
        }


        //printDebug(args);
    }
    
    
    if(!foundRW){
        cout << "-R/-W is a mandatory flag!" << endl;
        return false;
    }

    return true;
}

