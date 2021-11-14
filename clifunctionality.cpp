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
    

    for(auto& str : splitInput){
        if((str == "-R") && (!foundRW)){
            foundRW = true;
            cout << "found -R \n";
        }
        else if((str == "-W")&&(!foundRW)){
            foundRW = true;
            cout << "found -W \n";
        }
        else if(0){
            
        }
        else{
            return false;
        }

    }
    


    return true;
}

