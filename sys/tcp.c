/* File : tcp.c
Author : Aykow
date : 13 mai 2015

*/

#include "defs.h"
#include "net.h"
#include "tcp.h"

/******** VARIABLE GLOBAL ***********/

extern DHCP dhcp;
TCP tcp;

/******* END VARIABLE GLOBAL *******/


/********** DEFINITION DES FONCTIONS *********/

// Gère l'initialisation de la structure.
void init_tcp(void){
	tcp.state = ST_ASK_OPEN;
	tcp.dst_port = 0x0000;
	tcp.src_port = 0x0000;
	uint8_t i = 0;
    for(i=0;i<4;i++){
        tcp.ip_distant[i] = 0x00;
    }
    for(i=0;i<6;i++){
        tcp.mac_routeur[i] = 0x00;
    }
	tcp.flag = TCP_FLAG_SYN_V; // On demandera forcément à se synchronisé au départ
	tcp.seq_num = 0x00000000;
	tcp.ack_num = 0x00000000;
}

// Gère l'ouverture d'une session TCP.
int open_tcp(uint8_t *buf){
	
return 0;
}
// Gère la fermeture d'une session TCP.
int close_tcp(uint8_t *buf){

return 0;
}
// Gère l'envoie d'une trames http.
void http_send(uint8_t *buf,uint8_t *data){


}
// Gère la reception d'une trames http. 
void http_recv(uint8_t *buf){


}
// Gère la reception d'une trames TCP et réoriente si besoin vers http.
void tcp_recv(uint8_t *buf){
// On récupère le flag pour réorienté 
	tcp.flag = buf[TCP_FLAGS_P];
// On lit le flag pour réorienté ! 
switch(tcp.flag){
		case TCP_FLAG_SYN_V: // demande de synchronisation.
			make_tcp_synack_from_syn(buf);
		break;
		case TCP_FLAGS_SYNACK_V: // Confirmation de la demande de synchronisation
			make_tcp_syn(buf,tcp.mac_routeur,tcp.ip_distant);
		break;
		case TCP_FLAGS_ACK_V: // Data en reception
		// TO DO : Voir quelle donnée on a en reception.
		break;
		case TCP_FLAG_RST_V: // Demande de RST
		// TO DO : Trouver ce qu'il faut faire dans ce cas.
		break;
	}
}

// Gère les envoie paquets TCP.
void tcp_send(uint8_t * buf,uint8_t * data){

}


uint32_t get_seq_num(uint8_t *buf){
	uint32_t tmp = buf[TCP_SEQ_H_P]<<8 & 0x0000ff00 | buf[TCP_SEQ_H_P+1] & 0x000000ff;
        tmp = tmp << 8  & 0x00ffff00| buf[TCP_SEQ_H_P+2] & 0x000000ff;
        tmp = tmp << 8 & 0xffffff00 | buf[TCP_SEQ_H_P+3] & 0x000000ff; 
        //return  ((buf[TCP_SEQ_H_P]<<8) & 0x0000ff00);
        return tmp;
}
uint32_t get_ack_num(uint8_t *buf){
        uint32_t tmp = buf[TCP_SEQACK_P]<<8 & 0x0000ff00 | buf[TCP_SEQACK_P+1] & 0x000000ff;
        tmp = tmp << 8  & 0x00ffff00| buf[TCP_SEQACK_P+2] & 0x000000ff;
        tmp = tmp << 8 & 0xffffff00 | buf[TCP_SEQACK_P+3] & 0x000000ff; 
	return tmp;        
}

/******** END DEFINITION DE FONCTIONS **************/