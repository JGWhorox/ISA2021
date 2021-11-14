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
#include <algorithm>

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
        if((*it == "-R") && (!foundRW)){
            foundRW = true;
            cout << "found -R \n";
        }
        else if((*it == "-W") && (!foundRW)){
            foundRW = true;
            cout << "found -W \n";
        }
        else if(*it == "-d"){
            if(it+1 != splitInput.end()){
                args.filePath = *(++it);
            }
        }
        else{
            return false;
        }

    }
    
    //cout << args.filePath;

    return true;
}

