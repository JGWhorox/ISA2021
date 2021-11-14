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

bool readFromServer(const Arguments& args);


#endif