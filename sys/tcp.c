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
// return : l'état de la connexion.
uint8_t open_tcp(uint8_t *buf,TCP *tcp){
	make_tcp_syn(buf,tcp->mac_routeur,tcp->ip_distant,PORT_HTTP_SERVER,PORT_HTTP_ME);
	do{
		if(!(packetlen = enc28j60PacketReceive(BUFFER_SIZE, buf))){
			if(nb_recv > 50){
				conn->state = ST_ASK_OPEN;
				return conn->state;
			}
		}else{
			nb_recv++;
			switch(tcp_recv(buf,packetlen,conn)){
				case ST_ASK_OPEN:

					return conn->state;
					break;
				case ST_CONN :
					return conn->state;
					break;

			}
		}
	}while(1)
}
// Gère la fermeture d'une session TCP.
int close_tcp(uint8_t *buf){
	make_tcp_fin(buf,tcp->mac_routeur,tcp->ip_distant,PORT_HTTP_SERVER,PORT_HTTP_ME);
		if(!(packetlen = enc28j60PacketReceive(BUFFER_SIZE, buf))){
			if(nb_recv > 50){
				conn->state = ST_ASK_CLOSE;
				return conn->state;
			}
		}else{
			nb_recv++;
			switch(tcp_recv(buf,packetlen,conn)){
				case ST_ASK_CLOSE:

					return conn->state;
					break;
				case ST_DISCONN :
					return conn->state;
					break;

			}
		}
	}while(1)

}
// Gère l'envoie d'une trames http.
void http_send(uint8_t *buf,uint8_t *data){

	fill_tcp_data(buf,0, data);
	make_tcp_ack_with_data(buf,100);

}
// Gère la reception d'une trames http. 
void http_recv(uint8_t *buf){


}
// Gère la reception d'une trames TCP et réoriente si besoin vers http.
uint8_t tcp_recv(uint8_t *buf,TCP * conn){
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
						// et on est alors connecter :
						conn->state = ST_CONN;
					}
					break;
				case TCP_FLAG_RST_V:
					// On doit alors recommencer... 
					conn->state = ST_ASK_OPEN;
					break;
				case TCP_FLAGS_FIN_V | TCP_FLAG_ACK_V :
					make_tcp_ack_from_any(buf);
					conn->state = ST_DISCONN;
					break;

			}

			return conn->state;
		}
	}
	return 0;
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
	uint8_t retour;
	// TO DO: Activé le filtre TCP + ajout du timer.
	while(open_tcp(buf,&conn) != ST_CONN);
	// On peut alors envoyer les données reçu au serveur.
	char * data = "GET / HTTP/1.1\r\nHost: 192.168.0.33\r\nConnection: keep-alive\r\n\r\n";
	http_send(buf,data);
	while(close_tcp(buf,&conn) != ST_DISCONN);
}

/******** END DEFINITION DE FONCTIONS **************/