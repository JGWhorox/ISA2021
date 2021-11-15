/**
 * author Johann A. Gawron - xgawro00
 * file tftpSocket.cpp
 * brief This file includes socket and buffer creation and the logic for communicating with server
 */

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
//custom
#include "tftpSocket.h"
#include "pcapDataTypes.h"
#include "clifunctionality.h"


using namespace std;



static uint8_t udpBuffer[UINT16_MAX]; //max size of udp

const static char modeBinary[] = "octet";
const static char modeAscii[] = "netascii";
const static char transfer[] = "tsize";
const static char blck[] = "blksize";


bool readFromServer(const Arguments& args){

    struct sockaddr_in server_addr;

    int sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (sock < 0){
        return false;
    }
    timeval timeout = {};
    timeout.tv_usec = args.timeout*1000000;

    setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,reinterpret_cast<const char*>(&timeout), sizeof (timeout)); 

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(args.port);
    
    if(inet_pton(AF_INET, args.address.c_str(), &server_addr.sin_addr)<=0){
        cerr << "\nInvalid address/ Address not supported \n";
        return false;
    }

    uint8_t* index = udpBuffer;

    *((uint16_t*)udpBuffer) = SWAP((uint16_t)tftpOpcode::RRQ);
    index += 2;

    memcpy(index,args.filePath.c_str(),args.filePath.length()+1);
    index+= args.filePath.length()+1;

    if(args.binaryMode){
        memcpy(index, modeBinary, sizeof(modeBinary));
        index += sizeof(modeBinary);
    }
    else{
        memcpy(index, modeAscii, sizeof(modeAscii));
        index += sizeof(modeAscii);
    }

    memcpy(index, transfer, sizeof(transfer));
    index += sizeof(transfer);

    *(uint16_t*)index = SWAP('0'<<8 | 0);
    index += 2;


    //if there's moidified value of blocksize then modify RRQ packet
    if(args.blockSize > 512){        
        memcpy(index, blck, sizeof(blck));
        index += sizeof(blck);
        //converting blockSize in bytes to octets
        string str = to_string(args.blockSize);
        cout << str << endl;
        memcpy(index,str.c_str(),str.length()+1);
        index += str.length()+1;
    }


    string filename = args.filePath.substr(args.filePath.find_last_of("/")+1,args.filePath.length());

    ofstream myfile(filename);

    auto result = sendto (sock, udpBuffer, index-udpBuffer, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));


    cout << "Requesting read from server: " << args.address << " on port: " << args.port << endl;

    if (result > 0){      
        int n;
        uint64_t x = 0;
        cout << "Receiving DATA:" << endl;
        do{
            //we need to get the port in the response to know where to send ACK
            sockaddr_in session; 
            int len = sizeof(session);
            // we get port for the service in the response (1st data packet)
            n = recvfrom(sock, udpBuffer, sizeof(udpBuffer), 0, (struct sockaddr *)&session, (socklen_t *)&len);

            uint16_t filesize = strtol((char *)(udpBuffer+2+sizeof(transfer)),NULL,10);

            *(uint16_t*)udpBuffer = SWAP((uint16_t)tftpOpcode::ACK);

            *(uint16_t*)(udpBuffer+2) = (uint16_t)0;

            result = sendto (sock, udpBuffer, 4, 0, (struct sockaddr *)&session, sizeof(session));

            n = recvfrom(sock, udpBuffer, sizeof(udpBuffer), 0, (struct sockaddr *)&session, (socklen_t *)&len);
            
            if((args.blockSize != 512) && (*(uint16_t*)udpBuffer == SWAP((uint16_t)tftpOpcode::OPT))){
                
                *(uint16_t*)udpBuffer = SWAP((uint16_t)tftpOpcode::ACK);

                *(uint16_t*)(udpBuffer+2) = 0;

                result = sendto (sock, udpBuffer, 4, 0, (struct sockaddr *)&session, sizeof(session));
                if(result){
                    n = recvfrom(sock, udpBuffer, sizeof(udpBuffer), 0, (struct sockaddr *)&session, (socklen_t *)&len);    
                }
                
            }
            myfile.write((const char*)(udpBuffer+sizeof(tftpRcv)),n-4);
            
            x += n-4;
            
            cout << x << "B transferred" << "\t\r" << flush;
                      
            *(uint16_t*)udpBuffer = SWAP((uint16_t)tftpOpcode::ACK);

            sendto (sock, udpBuffer, 4, 0, (struct sockaddr *)&session, sizeof(session));

        }while(n == args.blockSize+4); // add blocksize later
        
        myfile.close();
        cout << endl;
    }
    return true;
}

bool readFromServerInIPv6(const Arguments& args){

    struct sockaddr_in6 server_addr;
    
    int sock = socket(AF_INET6,SOCK_DGRAM,IPPROTO_UDP);
    if (sock < 0){
        return false;
    }
    timeval timeout = {};
    timeout.tv_usec = args.timeout*1000000;

    setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,reinterpret_cast<const char*>(&timeout), sizeof (timeout)); 

    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(args.port);
    
    if(inet_pton(AF_INET6, args.address.c_str(), &server_addr.sin6_addr)<=0){
        cerr << "\nInvalid address/ Address not supported \n";
        return false;
    }

    uint8_t* index = udpBuffer;

    *((uint16_t*)udpBuffer) = SWAP((uint16_t)tftpOpcode::RRQ);
    index += 2;

    memcpy(index,args.filePath.c_str(),args.filePath.length()+1);
    index+= args.filePath.length()+1;

    if(args.binaryMode){
        memcpy(index, modeBinary, sizeof(modeBinary));
        index += sizeof(modeBinary);
    }
    else{
        memcpy(index, modeAscii, sizeof(modeAscii));
        index += sizeof(modeAscii);
    }

    memcpy(index, transfer, sizeof(transfer));
    index += sizeof(transfer);

    *(uint16_t*)index = SWAP('0'<<8 | 0);
    index += 2;


    //if there's moidified value of blocksize then modify RRQ packet
    if(args.blockSize > 512){        
        memcpy(index, blck, sizeof(blck));
        index += sizeof(blck);
        //converting blockSize in bytes to octets
        string str = to_string(args.blockSize);
        cout << str << endl;
        memcpy(index,str.c_str(),str.length()+1);
        index += str.length()+1;
    }

    string filename = args.filePath.substr(args.filePath.find_last_of("/")+1,args.filePath.length());

    ofstream myfile(filename);

    auto result = sendto (sock, udpBuffer, index-udpBuffer, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));


    cout << "Requesting read from server: " << args.address << " on port: " << args.port << endl;

    if (result > 0){      
        int n;
        uint64_t x = 0;
        cout << "Receiving DATA:" << endl;
        do{
            //we need to get the port in the response to know where to send ACK
            sockaddr_in6 session; 
            int len = sizeof(session);
            // we get port for the service in the response (1st data packet)
            n = recvfrom(sock, udpBuffer, sizeof(udpBuffer), 0, (struct sockaddr *)&session, (socklen_t *)&len);

            uint16_t filesize = strtol((char *)(udpBuffer+2+sizeof(transfer)),NULL,10);

            *(uint16_t*)udpBuffer = SWAP((uint16_t)tftpOpcode::ACK);

            *(uint16_t*)(udpBuffer+2) = (uint16_t)0;

            result = sendto (sock, udpBuffer, 4, 0, (struct sockaddr *)&session, sizeof(session));

            n = recvfrom(sock, udpBuffer, sizeof(udpBuffer), 0, (struct sockaddr *)&session, (socklen_t *)&len);
            
            if((args.blockSize != 512) && (*(uint16_t*)udpBuffer == SWAP((uint16_t)tftpOpcode::OPT))){
                
                *(uint16_t*)udpBuffer = SWAP((uint16_t)tftpOpcode::ACK);

                *(uint16_t*)(udpBuffer+2) = 0;

                result = sendto (sock, udpBuffer, 4, 0, (struct sockaddr *)&session, sizeof(session));
                if(result){
                    n = recvfrom(sock, udpBuffer, sizeof(udpBuffer), 0, (struct sockaddr *)&session, (socklen_t *)&len);    
                }
                
            }
            
            myfile.write((const char*)(udpBuffer+sizeof(tftpRcv)),n-4);
            
            x += n-4;
            
            cout << x << "B transferred" << "\t\r" << flush;
                      
            *(uint16_t*)udpBuffer = SWAP((uint16_t)tftpOpcode::ACK);

            sendto (sock, udpBuffer, 4, 0, (struct sockaddr *)&session, sizeof(session));

        }while(n == args.blockSize+4); // add blocksize later
        
        myfile.close();
        cout << endl;
    }
    return true;
}

bool writeToServer(const Arguments& args){
    struct sockaddr_in server_addr;

    int sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (sock < 0){
        return false;
    }
    timeval timeout = {};
    timeout.tv_usec = args.timeout*1000000;

    setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,reinterpret_cast<const char*>(&timeout), sizeof (timeout)); 
    
    uint8_t* index = udpBuffer;

    *((uint16_t*)udpBuffer) = SWAP((uint16_t)tftpOpcode::WRQ);
    index += 2;

    memcpy(index,args.filePath.c_str(),args.filePath.length()+1);
    index+= args.filePath.length()+1;

    if(args.binaryMode){
        memcpy(index, modeBinary, sizeof(modeBinary));
        index += sizeof(modeBinary);
    }
    else{
        memcpy(index, modeAscii, sizeof(modeAscii));
        index += sizeof(modeAscii);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(args.port);
    
    if(inet_pton(AF_INET, args.address.c_str(), &server_addr.sin_addr)<=0){
        cerr << "\nInvalid address/ Address not supported \n";
        return false;
    }

    //string filename = args.filePath.substr(args.filePath.find_last_of("/")+1,args.filePath.length());

    ifstream myfile(args.filePath,ios::binary);
    auto result = sendto (sock, udpBuffer, index-udpBuffer, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));


    cout << "Requesting write to server: " << args.address << " on port: " << args.port << endl;
    

    if (result > 0){      
        
        int n;
        //we need to get the port in the response to know where to send ACK
        sockaddr_in session; 
        int len = sizeof(session);
        // we get port for the service in the response (1st data packet)
        n = recvfrom(sock, udpBuffer, sizeof(udpBuffer), 0, (struct sockaddr *)&session, (socklen_t *)&len);

        if(*((uint16_t*)udpBuffer) == SWAP((uint16_t)tftpOpcode::ACK)){
            cout << "ACK from server, transmitting data..." << endl;   
        }

        uint64_t x = 0;
        uint16_t blockcount = 0;
        
        do{
            index = udpBuffer;

            *(uint16_t*)udpBuffer = SWAP((uint16_t)tftpOpcode::DAT);
            index += 2;

            blockcount++;

            *(uint16_t*)index = SWAP(blockcount);
            index += 2;
            
            char tmpBuffer[args.blockSize];

            myfile.read(tmpBuffer, sizeof(tmpBuffer));

            memcpy(index,tmpBuffer,myfile.gcount());

           
            index += myfile.gcount();
            
            
            int res = sendto (sock, udpBuffer, index-udpBuffer, 0, (struct sockaddr *)&session, sizeof(session));
            if(res){
                n = recvfrom(sock, udpBuffer, sizeof(udpBuffer), 0, (struct sockaddr *)&session, (socklen_t *)&len);
                if(!n){
                    return false;
                }
                if(!(*((uint16_t*)udpBuffer) == SWAP((uint16_t)tftpOpcode::ACK))){
                    cerr << "server didnt send ACK back" << endl;
                    return false;
                }
                if(*((uint16_t*)udpBuffer) == SWAP((uint16_t)tftpOpcode::ACK)){
                    x += myfile.gcount();
                    cout << x << "B transferred" << "\t\r" << flush;
                }
            }

            
        }while(myfile); // myfile returns false, when he couldn't fill the entire buffer given to him in read
        
        myfile.close();
        cout << endl;
    }
    return true;
}

bool writeToServerInIPv6(const Arguments& args){
    struct sockaddr_in6 server_addr;

    int sock = socket(AF_INET6,SOCK_DGRAM,IPPROTO_UDP);
    if (sock < 0){
        return false;
    }
    timeval timeout = {};
    timeout.tv_usec = args.timeout*1000000;

    setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,reinterpret_cast<const char*>(&timeout), sizeof (timeout)); 

    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(args.port);
    
    if(inet_pton(AF_INET6, args.address.c_str(), &server_addr.sin6_addr)<=0){
        cerr << "\nInvalid address/ Address not supported \n";
        return false;
    }

    uint8_t* index = udpBuffer;

    *((uint16_t*)udpBuffer) = SWAP((uint16_t)tftpOpcode::WRQ);
    index += 2;

    memcpy(index,args.filePath.c_str(),args.filePath.length()+1);
    index+= args.filePath.length()+1;

    if(args.binaryMode){
        memcpy(index, modeBinary, sizeof(modeBinary));
        index += sizeof(modeBinary);
    }
    else{
        memcpy(index, modeAscii, sizeof(modeAscii));
        index += sizeof(modeAscii);
    }

    ifstream myfile(args.filePath,ios::binary);
    auto result = sendto (sock, udpBuffer, index-udpBuffer, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));


    cout << "Requesting write to server: " << args.address << " on port: " << args.port << endl;
    

    if (result > 0){      
        
        int n;
        //we need to get the port in the response to know where to send ACK
        sockaddr_in6 session; 
        int len = sizeof(session);
        // we get port for the service in the response (1st data packet)
        n = recvfrom(sock, udpBuffer, sizeof(udpBuffer), 0, (struct sockaddr *)&session, (socklen_t *)&len);

        if(*((uint16_t*)udpBuffer) == SWAP((uint16_t)tftpOpcode::ACK)){
            cout << "ACK from server, transmitting data..." << endl;   
        }

        uint64_t x = 0;
        uint16_t blockcount = 0;
        
        do{
            index = udpBuffer;

            *(uint16_t*)udpBuffer = SWAP((uint16_t)tftpOpcode::DAT);
            index += 2;

            blockcount++;

            *(uint16_t*)index = SWAP(blockcount);
            index += 2;
            
            char tmpBuffer[args.blockSize];

            myfile.read(tmpBuffer, sizeof(tmpBuffer));

            memcpy(index,tmpBuffer,myfile.gcount());

           
            index += myfile.gcount();
            
            
            int res = sendto (sock, udpBuffer, index-udpBuffer, 0, (struct sockaddr *)&session, sizeof(session));
            if(res){
                n = recvfrom(sock, udpBuffer, sizeof(udpBuffer), 0, (struct sockaddr *)&session, (socklen_t *)&len);
                if(!n){
                    return false;
                }
                if(!(*((uint16_t*)udpBuffer) == SWAP((uint16_t)tftpOpcode::ACK))){
                    cerr << "server didnt send ACK back" << endl;
                    return false;
                }
                if(*((uint16_t*)udpBuffer) == SWAP((uint16_t)tftpOpcode::ACK)){
                    x += myfile.gcount();
                    cout << x << "B transferred" << "\t\r" << flush;
                }
            }

            
        }while(myfile); // myfile returns false, when he couldn't fill the entire buffer given to him in read
        
        myfile.close();
        cout << endl;
    }
    return true;
}
