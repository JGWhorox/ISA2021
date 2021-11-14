/**
 * author Johann A. Gawron - xgawro00
 * file argparser.h
 * brief main program declarations
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


using namespace std;


int main(int argc, char** argv){

    Arguments args;
    
    /*
    string test = "empty string are boring    tessting";

    vector<string> res = split(test);
    
    for(auto& str : res){
        cout << str << endl;
    }
    */

    while(1){
        string input;
        getline(cin, input);
        if(parseArgs(args, input)){
            //write 
            if(args.write){ 

            }
            //read
            else{

            }
            
        }
        else{
            cout << "Given arguments are incorrect!!!" << endl;
            printHelp();
            break;
        }
    }
    return 0;



    /*int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];
    int server_struct_length = sizeof(server_addr);
    
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(69);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Get input from the user:
    printf("Enter message: ");
    gets(client_message);
    
    // Send the message to server:
    if(sendto(socket_desc, client_message, strlen(client_message), 0,
         (struct sockaddr*)&server_addr, server_struct_length) < 0){
        printf("Unable to send message\n");
        return -1;
    }
    
    // Receive the server's response:
    if(recvfrom(socket_desc, server_message, sizeof(server_message), 0,
         (struct sockaddr*)&server_addr, &server_struct_length) < 0){
        printf("Error while receiving server's msg\n");
        return -1;
    }
    
    printf("Server's response: %s\n", server_message);
    
    // Close the socket:
    close(socket_desc);*/
}