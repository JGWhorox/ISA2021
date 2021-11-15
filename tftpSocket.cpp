#include <arpa/inet.h>
#include <sys/socket.h>
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


bool readFromServer(const Arguments& args){

    struct sockaddr_in server_addr;

    int sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (sock < 0){
        return false;
    }
    timeval timeout = {};
    timeout.tv_usec = args.timeout*1000000;

    setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,reinterpret_cast<const char*>(&timeout), sizeof (timeout)); 
    
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
    cout << udpBuffer+1 << endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(args.port);
    
    if(inet_pton(AF_INET, args.address.c_str(), &server_addr.sin_addr)<=0) 
    {
        cerr << "\nInvalid address/ Address not supported \n";
        return -1;
    }

    string filename = args.filePath.substr(args.filePath.find_last_of("/")+1,args.filePath.length());

    ofstream myfile(filename);
    auto result = sendto (sock, udpBuffer, index-udpBuffer, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (result > 0){      
        int n;
        do{
            //we need to get the port in the response to know where to send ACK
            sockaddr_in client; 
            int len = sizeof(client);
            
            n = recvfrom(sock, udpBuffer, sizeof(udpBuffer), 0, (struct sockaddr *)&client, (socklen_t *)&len);

            cout << n << endl;
            
            myfile.write((const char*)(index+sizeof(tftpRcv)),n-4);
            
            cout << "received block n: " <<  SWAP((uint16_t)((tftpRcv*)udpBuffer)->blockid) << endl;           

            *(uint16_t*)udpBuffer = SWAP((uint16_t)tftpOpcode::ACK);

            sendto (sock, udpBuffer, 4, 0, (struct sockaddr *)&client, sizeof(client));

        }while(n == args.blockSize+4); // add blocksize later
        myfile.close();
    }

    /*if(*((uint16_t*)udpBuffer) == SWAP((uint16_t)tftpOpcode::ACK)){
        cout << "ACK data transferring" << endl;   
    }*/
    
    

    return true;
}
