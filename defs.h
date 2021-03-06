#ifndef DEFS_H
#define DEFS_H
#include <ioCC1110.h>

/*
#define UIP_ETHADDR0 0x6c
#define UIP_ETHADDR1 0x05
#define UIP_ETHADDR2 0x07
#define UIP_ETHADDR3 0x0b
#define UIP_ETHADDR4 0x82
#define UIP_ETHADDR5 0xc3
*/

#define UIP_ETHADDR0 0xB8
#define UIP_ETHADDR1 0x28
#define UIP_ETHADDR2 0xEB
#define UIP_ETHADDR3 0x4D
#define UIP_ETHADDR4 0x8D
#define UIP_ETHADDR5 0xBB

typedef   signed char     int8_t;
typedef   signed short    int16_t;
typedef   signed long     int32_t;

typedef   unsigned char   uint8_t;
typedef   unsigned short  uint16_t;
typedef   unsigned long   uint32_t;

#define BUFFER_SIZE         1000
static uint8_t buffer[BUFFER_SIZE + 1];

// Host :
#define HOST_NAME "VICTORIA"

static uint8_t dhcp_ask; // 0 : On demande une IP ! 1: On a notre IP.


#define PORT_HTTP_SERVER	80
#define PORT_HTTP_ME		47395


// Structure du TCP
typedef struct TCP_STRUCT
{
	uint8_t state;
	uint16_t dst_port;
	uint16_t src_port;
	uint8_t ip_distant[4];
	uint8_t mac_routeur[6];
	uint16_t flag;
	uint32_t seq_num;
	uint32_t ack_num;
}TCP;


// Structure du dhcp :
typedef struct DHCP_STRUCT
{
	uint8_t state;
	uint8_t ip_routeur[4];
	uint8_t mac_routeur[6];
	uint8_t ip_give[4];
} DHCP;

#endif