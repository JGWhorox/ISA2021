/**
 * author Johann A. Gawron - xgawro00
 * file argparser.h
 * brief implementation of various CLI functionalities as defined by assignment
 */

#ifndef CLIFUNCTIONALITY_H
#define CLIFUNCTIONALITY_H

#include <string>

struct Arguments {
    bool write = false; // if not write then read
    std::string filePath = "";
    int timeout = 5; //in seconds
    int blockSize = 256;
    bool muticast = false;
    int mode = 0; // 0 - not assigned, 1 ascii or netascii, 2 binary or octet
    std::string address = "127.0.0.1,69";   
};


std::vector<std::string> split(std::string input);

void printHelp();

bool parseArgs(Arguments &args, std::string input);

#endif