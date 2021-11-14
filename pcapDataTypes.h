#ifndef PCAPDATATYPES_H
#define PCAPDATATYPES_H

#include <cstdint>

#define ETHERNET_TYPE_IPV4 0x0800
#define ETHERNET_TYPE_IPV6 0x86DD
#define ETHERNET_TYPE_VLAN 0x8100

#define IP_PROTOCOL_TCP 0x06
#define IP_PROTOCOL_UDP 0x11
#define IP_PROTOCOL_IPV6_ROUTE 0x2B
#define IP_PROTOCOL_IPV6_FRAG 0x2C
#define IP_PROTOCOL_IPV6_NONXT 0x3B
#define IP_PROTOCOL_IPV6_OPTS 0x3C
#define IP_PROTOCOL_HIP 0x8B
#define IP_PROTOCOL_SHIM6 0x8C
#define IP_PROTOCOL_ESP 0x32
#define IP_PROTOCOL_AH 0x33

#define TLS_PROTOCOL_HANDSHAKE 0x16


#define  TYPE_IPV4 0
#define  TYPE_IPV6 1

#define SWAP(num) (((num>>8) & 0xFF) | ((num&0xFF)<<8)) // swaps 2 lower bytes 

/*typedef struct {
    uint64_t array[2];
} ipv6Address;
typedef uint32_t ipv4Address;

//custom type for IP addr
typedef struct {
    uint8_t type;
    union {
        ipv6Address v6;
        ipv4Address v4;
    } addr;
}ipAddress;*/

// contains information about packet 
/*typedef struct {
    ipAddress srcAddr;
    ipAddress dstAddr;
    int32_t srcPort;
    uint16_t TLSBytes;
} TpacketDetail;*/

#pragma pack(push, 1)

struct ethernet2_header{
    uint8_t dest[6];
    uint8_t source[6];
    uint16_t type;
};

struct ipv4_header{
    uint8_t hlen : 4,
        ver : 4;
    uint8_t tos;
    uint16_t len;
    uint16_t id;
    uint16_t flags_frag;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t csum;
    uint32_t srcAddr;
    uint32_t dstAddr;
};

struct ipv6_header{
    uint32_t ver : 4,
        priority_traffic : 8,
        flow : 20;
    uint16_t len;
    uint8_t  nextHeader;
    uint8_t  hopLimit;
    uint64_t srcAddrV6[2];
    uint64_t dstAddrV6[2];
};

struct ipv6_ext_header{
    uint8_t nextHeader;
    uint8_t extLength;
    uint16_t options;
    uint32_t padding;
};

struct udpHeader{
    uint16_t srcPort;
    uint16_t dstPort;
    uint16_t length;
    uint16_t checksum;
};

#pragma pack(pop)

enum tftpOpcode: uint16_t{
    RRQ = 1,
    WRQ,
    ACK,
    DAT,
    ERR
};



#endif