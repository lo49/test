/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher 
 * Copyright: GPL V2
 *
 * Based on the net.h file from the AVRlib library by Pascal Stang.
 * For AVRlib See http://www.procyonengineering.com/
 * Used with explicit permission of Pascal Stang.
 *
 * Chip type           : ATMEGA88 with ENC28J60
 *********************************************/
 /*********************************************
 * Modified: nuelectronics.com -- Ethershield for Arduino
 *********************************************/

// notation: _P = position of a field
//           _V = value of a field

//@{

#ifndef NET_H
#define NET_H

#include "defs.h"

// ******* ETH *******
#define ETH_HEADER_LEN	14
// values of certain bytes:
#define ETHTYPE_ARP_H_V 0x08
#define ETHTYPE_ARP_L_V 0x06
#define ETHTYPE_IP_V	0x0800
#define ETHTYPE_IP_H_V  0x08
#define ETHTYPE_IP_L_V  0x00
// byte positions in the ethernet frame:
//
// Ethernet type field (2bytes):
#define ETH_TYPE_H_P 12
#define ETH_TYPE_L_P 13
//
#define ETH_DST_MAC 0
#define ETH_SRC_MAC 6


// ******* ARP *******
#define ETH_ARP_OPCODE_REPLY_H_V 0x0
#define ETH_ARP_OPCODE_REPLY_L_V 0x02
//
#define ETHTYPE_ARP_L_V 0x06
// arp.dst.ip
#define ETH_ARP_DST_IP_P 0x26
// arp.opcode
#define ETH_ARP_OPCODE_H_P 0x14
#define ETH_ARP_OPCODE_L_P 0x15
// arp.src.mac
#define ETH_ARP_SRC_MAC_P 0x16
#define ETH_ARP_SRC_IP_P 0x1c
#define ETH_ARP_DST_MAC_P 0x20
#define ETH_ARP_DST_IP_P 0x26

#define ARP_OPCODE_REQUEST_H_V	0x00
#define ARP_OPCODE_REQUEST_L_V	0x01
#define ARP_OPCODE_REPLY_H_V	0x00
#define ARP_OPCODE_REPLY_L_V	0x02

#define ARP_HARDWARE_TYPE_H_V	0x00
#define ARP_HARDWARE_TYPE_L_V	0x01
#define ARP_PROTOCOL_H_V		0x08
#define ARP_PROTOCOL_L_V		0x00
#define ARP_HARDWARE_SIZE_V		0x06
#define ARP_PROTOCOL_SIZE_V		0x04

#define ARP_HARDWARE_TYPE_H_P	0x0E
#define ARP_HARDWARE_TYPE_L_P	0x0F
#define ARP_PROTOCOL_H_P		0x10
#define ARP_PROTOCOL_L_P		0x11
#define ARP_HARDWARE_SIZE_P		0x12
#define ARP_PROTOCOL_SIZE_P		0x13
#define ARP_OPCODE_H_P			0x14
#define ARP_OPCODE_L_P			0x15
#define ARP_SRC_MAC_P			0x16
#define ARP_SRC_IP_P			0x1C
#define ARP_DST_MAC_P			0x20
#define ARP_DST_IP_P			0x26


// ******* IP *******
#define IP_HEADER_LEN		20

#define IP_PROTO_ICMP_V		0x01
#define IP_PROTO_TCP_V		0x06
#define IP_PROTO_UDP_V		0x11
#define IP_V4_V				0x40
#define IP_HEADER_LENGTH_V	0x05

#define IP_P				0x0E
#define IP_HEADER_VER_LEN_P	0x0E
#define IP_TOS_P			0x0F
#define IP_TOTLEN_H_P		0x10
#define IP_TOTLEN_L_P		0x11
#define IP_ID_H_P			0x12
#define IP_ID_L_P			0x13
#define IP_FLAGS_P 			0x14     
#define IP_FLAGS_H_P		0x14
#define IP_FLAGS_L_P		0x15
#define IP_TTL_P			0x16
#define IP_PROTO_P			0x17
#define IP_CHECKSUM_P 		0x18 
#define IP_CHECKSUM_H_P		0x18
#define IP_CHECKSUM_L_P		0x19
#define IP_SRC_IP_P			0x1A
#define IP_DST_IP_P			0x1E

#define IP_SRC_P 			0x1a
#define IP_DST_P 			0x1e
#define IP_HEADER_LEN_VER_P 0xe

#define IP_BROADCAST		0xFF
// ******* ICMP *******
#define ICMP_TYPE_ECHOREPLY_V 0
#define ICMP_TYPE_ECHOREQUEST_V 8
//
#define ICMP_TYPE_P 0x22
#define ICMP_CHECKSUM_P 0x24

// ******* UDP *******
#define UDP_HEADER_LEN	8
//
#define UDP_SRC_PORT_H_P 0x22
#define UDP_SRC_PORT_L_P 0x23
#define UDP_DST_PORT_H_P 0x24
#define UDP_DST_PORT_L_P 0x25
//
#define UDP_LEN_H_P 0x26
#define UDP_LEN_L_P 0x27
#define UDP_CHECKSUM_H_P 0x28
#define UDP_CHECKSUM_L_P 0x29
#define UDP_DATA_P 0x2a


// *********** BOOTSTRAP ***************
#define BOOTSTRAP_MESS_TYPE	0x2A
 #define BOOT_ACK 			0x05
 #define BOOT_REQUEST 		0x01
 #define BOOT_OFFER			0x02
 #define BOOT_DISCOVER		0x01
 #define BOOT_REPLY			0x02
#define BOOTSTRAP_HDW_TYPE	0x2B
 #define ETHERNET_TYPE		0x01
#define BOOTSTRAP_ADDR_LEN	0x2C
 #define LEN_ADDR			0x06
#define BOOTSTRAP_HOPS		0x2D
#define	BOOTSTRAP_TR_ID		0x2E // 4 bytes
#define BOOTSTRAP_SC_EL		0x32 // 2 bytes
#define	BOOTSTRAP_FLAGS		0x34 // 2 bytes
#define BOOTSTRAP_CLIENT_IP	0x36
#define BOOTSTRAP_YOUR_IP	0x3A // 4 bytes 
#define BOOTSTRAP_SERVER_IP	0x3E // 4 bytes
#define BOOTSTRAP_RELAY_IP	0x42 // 4 bytes
#define BOOTSTRAP_CLIENT_MAC 0x46 // 6 bytes
#define BOOTSTRAP_SVR_HST_NAME	0x56 
#define BOOTSTRAP_BOOT_FLE_NAME	0x96
#define BOOTSTRAP_MAGIC_COOKIE	0x116
#define CLIENT_MAC_ADRESS		0x46
#define LEN_CLIENT_MAC_ADRESS	0x06

#define OPTION_BEGIN	0x11A // Début des option !
#define	OPTION_SUBMASK		0x01
#define OPTION_ROUTEUR		0x03
#define OPTION_DNS_SERVER	0x06
#define OPTION_DOMAIN_NAME	0x0f
#define OPTION_REQUEST_IP	0x32 // Request IP Address
 #define LEN_REQUEST_IP		0x04
#define OPTION_MESS_TYPE	0x35 // DHCP Message Type
 #define LEN_MESS_TYPE		0x01
#define OPTION_SERVER_ID	0x36 // Server Identifier
 #define LEN_SERVER_ID		0x04
#define OPTION_CLIENT_ID	0x3D // client identifier
 #define LEN_CLIENT_ID		0x07
#define OPTION_END			0xFF
#define DHCP_DISCOVER		0x1
#define DHCP_OFFER			0x2
#define DHCP_REQUEST		0x3
#define DHCP_DECLINE		0x4
#define DHCP_ACK			0x5
#define DHCP_NACK			0x6
#define DHCP_RELEASE		0x7
#define DHCP_INFORM			0x8
#define HWR_ETHERNET		0x01
#define OPTION_HOST_NAME 			0x0C
#define LEN_HOST_NAME 		0x08
#define LEN_REQUEST_LIST	0x0D
#define OPTION_REQUEST_LIST 0x37
#define NETBIOS_NS			0x2C
#define NETBIOS_NT			0x2E
#define NETBIOS_SCOPE		0x2F
#define PERF_ROUTEUR_DISC	0x1F
#define STATIC_ROUTE		0x21
#define CLASSLESS_SR		0x79
#define PRIVATECLASSLESS_SR	0xF9
#define PRIVATEPROXY_AUTODISC	0xFC
#define VENDOR_SPE			0x2B
#define OPTION_VENDOR_CLASS_ID		0x3C
#define LEN_VENDOR_CLASS_ID		0x08
#define DHCP_SRV_ID				0x11D




// ******* TCP *******
//  plain len without the options:
#define TCP_HEADER_LEN_PLAIN 20

		
#define TCP_FLAG_FIN_V		0x01
#define TCP_FLAGS_FIN_V		0x01
#define TCP_FLAGS_SYN_V		0x02
#define TCP_FLAG_SYN_V		0x02
#define TCP_FLAG_RST_V		0x04
#define TCP_FLAG_PUSH_V		0x08
#define TCP_FLAGS_ACK_V		0x10
#define TCP_FLAG_ACK_V		0x10
#define TCP_FLAG_URG_V		0x20
#define TCP_FLAG_ECE_V		0x40
#define TCP_FLAG_CWR_V		0x80
#define TCP_FLAGS_SYNACK_V 	0x12

#define TCP_SRC_PORT_H_P 	0x22
#define TCP_SRC_PORT_L_P 	0x23
#define TCP_DST_PORT_H_P 	0x24
#define TCP_DST_PORT_L_P 	0x25
#define TCP_SEQ_P  			0x26	// the tcp seq number is 4 bytes 0x26-0x29
#define TCP_SEQ_H_P 		0x26
#define TCP_SEQACK_P  		0x2A	// 4 bytes
#define TCP_SEQACK_H_P 		0x2A
#define TCP_HEADER_LEN_P 	0x2E
#define TCP_FLAGS_P 		0x2F
#define TCP_FLAG_P			0x2F
#define TCP_WINDOWSIZE_H_P	0x30	// 2 bytes
#define TCP_WINDOWSIZE_L_P	0x31
#define TCP_CHECKSUM_H_P 	0x32
#define TCP_CHECKSUM_L_P 	0x33
#define TCP_URGENT_PTR_H_P 	0x34	// 2 bytes
#define TCP_URGENT_PTR_L_P 	0x35
#define TCP_OPTIONS_P 		0x36
#define TCP_DATA_P			0x36


//

#define ARP 			0x01
#define IP_TCP			0x02
#define	IP_UDP			0x03
#define	IP_ICMP			0x04





static uint8_t macaddr[6];
extern uint8_t ipaddr[4];
static uint8_t subMask[4];
static uint8_t routeur[4];
static uint8_t dns[4];
static uint32_t time_dhcp;


#endif
//@}

