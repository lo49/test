/* File : udp.h 
Author : Aykow
date : 13 mai 2015

*/

#ifndef UDP_H
#define UDP_H

#define ST_DHCP_ASK				0x01
#define ST_DHCP_OK				0x02
#define ST_DHCP_WAIT_OFFER 		0x03
#define ST_DHCP_WAIT_ACK		0x04
#define ST_DHCP_SEND_REQUEST	0x05

#include "ip_arp_udp_tcp.h"
#include "enc28j60.h"
#include "net.h"
#include "defs.h"
// Structure du dhcp :
typedef struct DHCP_STRUCT
{
	uint8_t state;
	uint8_t ip_routeur[4];
	uint8_t mac_routeur[6];
	uint8_t ip_give[4];
} DHCP;

// Gère la demande de requête au serveur DHCP
void dhcp_request(uint8_t *buf,uint16_t packetlen,const DHCP gest);
// Gère la demande de découverte
void dhcp_discover(uint8_t *buf,DHCP * gest);
// Gère la demande ACK
uint8_t dhcp_ack(uint8_t * buf);
// Gère les réponses du serveur DHCP
uint8_t dhcp_reponse(uint8_t *buf,uint16_t packetlen, DHCP *gest);
// Gère les étapes du DHCP :
DHCP dhcp(uint8_t * buf);


// Gère les paquets UDP
uint8_t udp(uint8_t * buf,uint16_t * packetLen);


void make_udp_header(uint16_t source_port, uint16_t dest_port, uint16_t longueur, uint8_t *buf);

void make_booststrap_protocol (uint8_t type_mess, uint16_t bootp_flags, uint8_t *buf );

void make_options(uint8_t mess_type, uint8_t *buf);
void make_options_request(const DHCP gest,uint8_t * buf);

#endif