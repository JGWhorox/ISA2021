/**
 * author Johann A. Gawron - xgawro00
 * file mytftpclient.h
 * brief main program declarations
 */

#ifndef MYTFTPCLIENT_H
#define MYTFTPCLIENT_H

#include <string>
#include <cstdint>

static inline uint16_t SWAP(uint16_t val) { return (((val << 8 ) & 0xFF00) | ((val >> 8 )& 0x00FF)); };

#pragma pack(push, 1)
struct tftpRcv{
    uint16_t opcode;
    uint16_t blockid;
};

#pragma pack(pop)

enum tftpOpcode: uint16_t{
    RRQ = 1,
    WRQ = 2,
    DAT = 3,
    ACK = 4,
    ERR = 5,
    OPT = 6
};


#endif