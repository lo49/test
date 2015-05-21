/* File : tcp.h 
Author : Aykow
date : 13 mai 2015

*/

#ifndef TCP_H
#define TCP_H

#include "defs.h"

#include "net.h"
#include "ip_arp_udp_tcp.h"

#define ST_CONN 		0x01
#define ST_DISCONN		0x02
#define ST_ASK_OPEN		0x03
#define ST_ASK_CLOSE    0x04


// Gère l'initialtisation de la structure.
void init_tcp(TCP *tcp, DHCP ip);
// Gère l'ouverture d'une session TCP.
void open_tcp(uint8_t *buf,TCP * tcp);
// Gère la fermeture d'une session TCP.
int close_tcp(uint8_t *buf);
// Gère l'envoie d'une trames http.
void http_send(uint8_t *buf,uint8_t *data);
// Gère la reception d'une trames http. 
void http_recv(uint8_t *buf);
// Gère la reception d'une trames TCP et réoriente si besoin vers http.
uint8_t tcp_recv(uint8_t *buf);
// Gère les paquets TCP.
TCP tcp(uint8_t* buf,DHCP ip);


uint32_t get_seq_num(uint8_t *buf);
uint32_t get_ack_num(uint8_t *buf);

#endif