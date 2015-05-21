/* File : tcp.c
Author : Aykow
date : 13 mai 2015

*/

#include "tcp.h"

/******** VARIABLE GLOBAL ***********/



/******* END VARIABLE GLOBAL *******/


/********** DEFINITION DES FONCTIONS *********/

// Gère l'initialisation de la structure.
void init_tcp(TCP *tcp,DHCP ip){
	tcp->state = ST_ASK_OPEN;
	tcp->dst_port = 0x0000;
	tcp->src_port = 0x0000;
	uint8_t i = 0;
    for(i=0;i<4;i++){
        tcp->ip_distant[i] = 0x00;
    }
    for(i=0;i<6;i++){
        tcp->mac_routeur[i] = ip.mac_routeur[i];
    }
	tcp->flag = TCP_FLAG_SYN_V; // On demandera forcément à se synchronisé au départ
	tcp->seq_num = 0x00000000;
	tcp->ack_num = 0x00000000;
}

// Gère l'ouverture d'une session TCP.
void open_tcp(uint8_t *buf,TCP tcp){
	make_tcp_syn(buf,tcp.mac_routeur,tcp.ip_distant,PORT_HTTP_SERVER,PORT_HTTP_ME);
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
void tcp_recv(uint8_t *buf,TCP * conn){
	// On vérifie le type de protocol : 
	if(buf[IP_PROTO_P] == IP_PROTO_TCP_V){
		// Les ports :
		if(buf[TCP_SRC_PORT_L_P] == PORT_HTTP_SERVER && buf[TCP_DST_PORT_H_P] == (PORT_HTTP_ME>>8&0xFF) && buf[TCP_DST_PORT_L_P] == (PORT_HTTP_ME &0xFF)){
			// On vérifie maintenant les flags : 
			switch(buf[TCP_FLAG]){
				case TCP_FLAG_SYN_V | TCP_FLAG_ACK_V :
					// Si l'on est en  état de demande de connection c'est normal
					if(conn->state == ST_ASK_OPEN){
						// On renvoie alors la réponse demandé.	
						make_tcp_ack_from_any(buf);
					}
					break;
				case TCP_FLAG_RST_V:
					// On doit alors recommencer... 
					conn->state = ST_ASK_OPEN;
					break;
			}
		}
	}
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


TCP tcp(uint8_t * buf,DHCP ip){
	TCP conn;
	init_tcp(&conn,ip);

	uint8_t packetlen = 0;
	uint8_t nb_recv = 0;
	open_tcp(buf,conn); // TO DO: Activé le filtre TCP + ajout du timer.
	while(1){
		if(!(packetlen = enc28j60PacketReceive(BUFFER_SIZE, buf))){
			if(nb_recv > 50){
				conn.state = ST_ASK_OPEN;
				return conn;
			}
		}else{
			nb_recv++;
			switch(tcp_recv(buf,packetlen,&conn)){
				case :
					break;
                        default:
                            break;
			}
		}
	}
}

/******** END DEFINITION DE FONCTIONS **************/