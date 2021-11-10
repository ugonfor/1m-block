#pragma once

#include <stdint.h>

#pragma pack(push, 1)
struct Tcphdr {
    uint16_t sport_;
    uint16_t dport_;
    uint32_t seq;
    uint32_t ack;
    uint8_t offset_; // offset : 4bit, reverved : 3bit, flag 1bit
    uint8_t flags; // flag 8bit
    uint16_t windows;
    uint16_t checksum;
    uint16_t urgent_ptr;
    
    uint16_t sport(){ return ntohs(sport_);}
    uint16_t dport(){ return ntohs(dport_);}
    uint8_t offset(){ return ((offset_ & 0xf0) >> 4) * 4; }
};
#pragma pack(pop)