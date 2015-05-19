/* File : udp.h 
Author : Aykow
date : 13 mai 2015

*/

#ifndef UDP_H
#define UDP_H

#define ST_DHCP_ASK		0x01
#define ST_DHCP_OK		0x02

// Structure du dhcp :
typedef struct DHCP_STRUCT
{
	uint8_t state;
	uint8_t ip_routeur[4];
	uint8_t mac_routeur[6];
	uint8_t ip_give[4];
} DHCP;

// Gère la demande de requête au serveur DHCP
void dhcp_request(uint8_t *buf,uint16_t packetlen,uint8_t *ipaddr_give,uint8_t* ipserver);
// Gère la demande de découverte
void dhcp_discover(uint8_t *buf);
// Gère la demande ACK
uint8_t dhcp_ack(uint8_t * buf);
// Gère les réponses du serveur DHCP
uint8_t dhcp_reponse(uint8_t *buf,uint16_t packetlen);


// Gère les paquets UDP
uint8_t udp(uint8_t * buf,uint16_t * packetLen);


void make_udp_header(uint16_t source_port, uint16_t dest_port, uint16_t longueur);

void make_booststrap_protocol (uint8_t type_mess, uint16_t bootp_flags);
#endif