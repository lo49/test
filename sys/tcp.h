/* File : tcp.h 
Author : Aykow
date : 13 mai 2015

*/

#ifndef TCP_H
#define TCP_H

#define ST_CONN 		0x01
#define ST_DISCONN		0x02
#define ST_ASK_OPEN		0x03
#define ST_ASK_CLOSE    0x04



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

// Gère l'initialtisation de la structure.
void init_tcp(void);
// Gère l'ouverture d'une session TCP.
int open_tcp(uint8_t *buf);
// Gère la fermeture d'une session TCP.
int close_tcp(uint8_t *buf);
// Gère l'envoie d'une trames http.
void http_send(uint8_t *buf,uint8_t *data);
// Gère la reception d'une trames http. 
void http_recv(uint8_t *buf);
// Gère la reception d'une trames TCP et réoriente si besoin vers http.
void tcp_recv(uint8_t *buf);
// Gère les paquets TCP.
void tcp_gest(uint8_t* buf);


uint32_t get_seq_num(uint8_t *buf);
uint32_t get_ack_num(uint8_t *buf);

#endif