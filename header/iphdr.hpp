#pragma once

#include <stdint.h>

#pragma pack(push, 1)
struct Iphdr {
	uint8_t ver_ihl;
	uint8_t tos;
	uint16_t len;
	uint16_t id;
	uint16_t flags_fragment_offset;
	uint8_t ttl;
	uint8_t protocol;
	uint16_t checksum;
	uint32_t sip;
	uint32_t dip;

	enum: uint8_t {
		tcp = 6 //tcp
	};
	
	uint8_t offset(){return (ver_ihl & 0xf)*4;}
};
#pragma pack(pop)