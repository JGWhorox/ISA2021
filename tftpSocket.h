/**
 * author Johann A. Gawron - xgawro00
 * file tftpSocket.h
 * brief This header file includes declaration for Read and Write functionality of tftp client
 */

#ifndef TFTPSOCKET_H
#define TFTPSOCKET_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <cstring>
//custom
#include "tftpSocket.h"
#include "pcapDataTypes.h"
#include "clifunctionality.h"

bool checkError(uint8_t* buffer);

bool readFromServer(Arguments& args);

bool readFromServerInIPv6(Arguments& args);

bool writeToServer(Arguments& args);

bool writeToServerInIPv6(Arguments& args);


#endif