/* File : udp.c 
Author : Aykow
date : 13 mai 2015

*/

// Gère la demande de découverte
#include "udp.h"
#include "ip_arp_udp_tcp.h"
#include "enc28j60.h"
#include "net.h"
#include "defs.h"

void dhcp_discover(uint8_t *buf,DHCP * gest){ 

	uint8_t dst_mac[6]={0xff,0xff,0xff,0xff,0xff,0xff} ;
	make_eth_ip_new(buf, dst_mac);

	// IP protocole

	uint8_t dst_ip[4]={255,255,255,255};
	make_ip_udp_new(buf, 328, dst_ip);
	fill_ip_hdr_checksum(buf);
	make_udp_header(68,67,308, buf);
	make_booststrap_protocol (BOOT_REQUEST, 0x8000, buf);
	make_options(BOOT_DISCOVER, buf);
	gest->state = ST_DHCP_WAIT_OFFER;
	enc28j60PacketSend(342, buf);
}


DHCP dhcp(uint8_t *buf){
	// On teste si l'on reçoit une réponse d'un serveur DHCP :
	
	uint8_t nbr_recv = 0;
	DHCP gest;
	// init de la structure dhcp.
	gest.state = ST_DHCP_ASK; 
	uint8_t i;
	for(i=0;i<4;i++){
		gest.ip_routeur[i] = 0x00;
		gest.ip_give[i] = 0x00;
	}
	for(i=0;i<6;i++){
		gest.mac_routeur[i] = 0x00;
	}
        uint8_t packetlen;
        dhcp_discover(buf,&gest); // TO DO : Activé le filtre dhcp.
	// on compte le nombre de paquet reçu; TO DO : Gérer l'attente par le biais du timer.
	while(1){
		if (!(packetlen = enc28j60PacketReceive(BUFFER_SIZE, buf))){
			// pas de réponse de paquet 
			//break;
			gest.state = ST_DHCP_ASK; 
			if(nbr_recv>50)
			{
			return gest; // On devra alors retenter.
			}
		}else{
			nbr_recv++;
			// On check la réponse :
			switch(dhcp_reponse(buf,packetlen, &gest)){
				case ST_DHCP_SEND_REQUEST : 
					dhcp_request(buf,packetlen,&gest);
					break;
				case ST_DHCP_OK:
					return gest;
					break;
				default : 
					break;
			}
		}
	}

}

void dhcp_request(uint8_t *buf,uint16_t packetlen,DHCP *gest){
	uint8_t dst_mac[6]={0xff,0xff,0xff,0xff,0xff,0xff};
	make_eth_ip_new(buf, dst_mac);

	// IP protocole

	uint8_t dst_ip[4]={255,255,255,255};
	make_ip_udp_new(buf, 328, dst_ip);
	fill_ip_hdr_checksum(buf);
	make_udp_header(68,67,308, buf);
	make_booststrap_protocol (BOOT_REQUEST, 0x8000, buf);
	make_options_request(*gest,buf);
	gest->state = ST_DHCP_WAIT_ACK;
	enc28j60PacketSend(342, buf);
}

void make_udp_header(uint16_t source_port, uint16_t dest_port, uint16_t longueur, uint8_t *buf)
{
	
	buf[UDP_SRC_PORT_H_P]= source_port>>8 & 0xff;
	buf[UDP_SRC_PORT_L_P]= source_port & 0xff;


	buf[UDP_DST_PORT_H_P] = dest_port>>8 & 0xff;
	buf[UDP_DST_PORT_L_P] = dest_port & 0xff;

	
	buf[UDP_LEN_H_P] = longueur>>8 & 0xff;
	buf[UDP_LEN_L_P] = longueur & 0xff;

	uint16_t ck= checksum(buf, longueur,1);
	buf[UDP_CHECKSUM_H_P] = ck>>8 & 0xff;
	buf[UDP_CHECKSUM_L_P] = ck & 0xff;

	// booststrap protocole
}

void make_booststrap_protocol (uint8_t type_mess, uint16_t bootp_flags, uint8_t *buf )
{

	buf[BOOTSTRAP_MESS_TYPE]= type_mess;
	buf[BOOTSTRAP_HDW_TYPE]= 0x01;
	buf[BOOTSTRAP_ADDR_LEN]= 0x06;
	buf[BOOTSTRAP_HOPS]= 0x00;
	buf[BOOTSTRAP_TR_ID]= 0x99;
	buf[BOOTSTRAP_TR_ID+1]= 0x994a6119;
	buf[BOOTSTRAP_TR_ID+2]= 0x99;
	buf[BOOTSTRAP_TR_ID+3]= 0x99;
	buf[BOOTSTRAP_SC_EL]= 0x0000;
	buf[BOOTSTRAP_FLAGS]= bootp_flags>>8 & 0xff;
	buf[BOOTSTRAP_FLAGS+1]=bootp_flags & 0xff;
	buf[BOOTSTRAP_CLIENT_IP]= 0x00000000;
	buf[BOOTSTRAP_YOUR_IP]=0x00000000;
	buf[BOOTSTRAP_SERVER_IP]=0x00000000;
	buf[BOOTSTRAP_RELAY_IP]= 0x00000000;
	buf[BOOTSTRAP_CLIENT_MAC]=UIP_ETHADDR0;
	buf[BOOTSTRAP_CLIENT_MAC+1]= UIP_ETHADDR1;
	buf[BOOTSTRAP_CLIENT_MAC+2]= UIP_ETHADDR2;
	buf[BOOTSTRAP_CLIENT_MAC+3]= UIP_ETHADDR3;
	buf[BOOTSTRAP_CLIENT_MAC+4]= UIP_ETHADDR4;
	buf[BOOTSTRAP_CLIENT_MAC+5]= UIP_ETHADDR5;
	buf[BOOTSTRAP_MAGIC_COOKIE]= 0x63;
	buf[BOOTSTRAP_MAGIC_COOKIE+1]=0x82;
	buf[BOOTSTRAP_MAGIC_COOKIE+2]=0x53;
	buf[BOOTSTRAP_MAGIC_COOKIE+3]=0x63;
}


void make_options(uint8_t mess_type, uint8_t *buf)
{
	// dhcp message type
	uint16_t adress = OPTION_BEGIN;
	buf[adress]=OPTION_MESS_TYPE;
	buf[++adress] = LEN_MESS_TYPE;
	buf[++adress] = mess_type;
	buf[++adress] = OPTION_CLIENT_ID;
	buf[++adress] = LEN_CLIENT_ID;
	buf[++adress] = HWR_ETHERNET;
	buf[++adress] = UIP_ETHADDR0;
	buf[++adress] = UIP_ETHADDR1;
	buf[++adress] = UIP_ETHADDR2;
	buf[++adress] = UIP_ETHADDR3;
	buf[++adress] = UIP_ETHADDR4;
	buf[++adress] = UIP_ETHADDR5;
	buf[++adress] = OPTION_HOST_NAME;
	buf[++adress] = LEN_HOST_NAME;
	buf[++adress] = 0x56;
	buf[++adress] = 0x49;
	buf[++adress] = 0x43;
	buf[++adress] = 0x54;
	buf[++adress] = 0x4F;
	buf[++adress] = 0x52;
	buf[++adress] = 0x49;
	buf[++adress] = 0x41;
	buf[++adress] = OPTION_VENDOR_CLASS_ID;
	buf[++adress] = LEN_VENDOR_CLASS_ID;
	uint8_t vendor_id[8]={0x4D, 0x53, 0x46, 0x54, 0x20, 0x35, 0x2E, 0x30};
	uint8_t i;
	for(i=0;i<8;i++){
		buf[++adress]=vendor_id[i];
	} 
		
	buf[++adress] = OPTION_REQUEST_LIST;
	buf[++adress] = LEN_REQUEST_LIST;
	buf[++adress] = OPTION_SUBMASK;
	buf[++adress] = OPTION_DOMAIN_NAME;
	buf[++adress] = OPTION_ROUTEUR;
	buf[++adress] = OPTION_DNS_SERVER;
	buf[++adress] = NETBIOS_NS;
	buf[++adress] = NETBIOS_NT;
	buf[++adress] = NETBIOS_SCOPE;
	buf[++adress] = PERF_ROUTEUR_DISC;
	buf[++adress] = STATIC_ROUTE;
	buf[++adress] = CLASSLESS_SR;
	buf[++adress] = PRIVATECLASSLESS_SR;
	buf[++adress] = PRIVATEPROXY_AUTODISC;
	buf[++adress] = VENDOR_SPE;
	buf[++adress] = OPTION_END;
}

void make_options_request(const DHCP gest,uint8_t * buf){
	// dhcp message type
	uint16_t adress = OPTION_BEGIN;
	uint8_t i;
	buf[adress]=OPTION_MESS_TYPE;
	buf[++adress] = LEN_MESS_TYPE;
	buf[++adress] = DHCP_REQUEST;
	buf[++adress] = OPTION_CLIENT_ID;
	buf[++adress] = LEN_CLIENT_ID;
	buf[++adress] = HWR_ETHERNET;
	buf[++adress] = UIP_ETHADDR0;
	buf[++adress] = UIP_ETHADDR1;
	buf[++adress] = UIP_ETHADDR2;
	buf[++adress] = UIP_ETHADDR3;
	buf[++adress] = UIP_ETHADDR4;
	buf[++adress] = UIP_ETHADDR5;
	buf[++adress] = OPTION_REQUEST_IP;
	buf[++adress] = LEN_REQUEST_IP;
	for(i=0;i<4;i++){
		buf[++adress] = gest.ip_give[i];
	}
	buf[++adress] = OPTION_SERVER_ID;
	buf[++adress] = LEN_SERVER_ID;
	for(i=0;i<4;i++){
		buf[++adress] = gest.ip_routeur[i];
	}
	buf[++adress] = OPTION_HOST_NAME;
	buf[++adress] = LEN_HOST_NAME;
	buf[++adress] = 0x56;
	buf[++adress] = 0x49;
	buf[++adress] = 0x43;
	buf[++adress] = 0x54;
	buf[++adress] = 0x4F;
	buf[++adress] = 0x52;
	buf[++adress] = 0x49;
	buf[++adress] = 0x41;
	buf[++adress] = OPTION_VENDOR_CLASS_ID;
	buf[++adress] = LEN_VENDOR_CLASS_ID;
	uint8_t vendor_id[8]={0x4D, 0x53, 0x46, 0x54, 0x20, 0x35, 0x2E, 0x30};
	for(i=0;i<8;i++){
		buf[++adress]=vendor_id[i];
	} 
		
	buf[++adress] = OPTION_REQUEST_LIST;
	buf[++adress] = LEN_REQUEST_LIST;
	buf[++adress] = OPTION_SUBMASK;
	buf[++adress] = OPTION_DOMAIN_NAME;
	buf[++adress] = OPTION_ROUTEUR;
	buf[++adress] = OPTION_DNS_SERVER;
	buf[++adress] = NETBIOS_NS;
	buf[++adress] = NETBIOS_NT;
	buf[++adress] = NETBIOS_SCOPE;
	buf[++adress] = PERF_ROUTEUR_DISC;
	buf[++adress] = STATIC_ROUTE;
	buf[++adress] = CLASSLESS_SR;
	buf[++adress] = PRIVATECLASSLESS_SR;
	buf[++adress] = PRIVATEPROXY_AUTODISC;
	buf[++adress] = VENDOR_SPE;
	buf[++adress] = OPTION_END;

}
// dhcp_reponse : Doit renvoyer l'état de la communication dhcp.
// En vérifiant que l'on est bien dans l'état : ST_DHCP_WAIT_OFFER
// si on a une offre -> ST_DHCP_SEND_REQUEST
// Et on enregistre les informations données : ip_give,ip_routeur,mac_routeur.
// TO DO : améliorer les informations données car parfois il y a  bail, etc.
uint8_t dhcp_reponse(uint8_t *buf,uint16_t packetlen, DHCP *gest){
// si protocole vaut 17, continuer, presence udp
	if (buf[IP_PROTO_P]==0x11) {
	 	if (buf[UDP_SRC_PORT_L_P] ==67 && buf[UDP_DST_PORT_L_P]==68) {
	 		if (buf[BOOTSTRAP_MESS_TYPE]==BOOT_REPLY){
	 			uint8_t i;
	 			uint8_t tab_mac[6]={UIP_ETHADDR0, UIP_ETHADDR1,UIP_ETHADDR2, UIP_ETHADDR3, UIP_ETHADDR4, UIP_ETHADDR5};
	 			for(i=0;i<LEN_CLIENT_MAC_ADRESS;i++)
	 			{
	 				if (buf[CLIENT_MAC_ADRESS+i]!=tab_mac[i]){return 0;}

	 			}
	 			uint16_t adress= OPTION_BEGIN;
	 			uint8_t len_adress;
	 			while(buf[adress]!=OPTION_END)
	 			{
	 				len_adress= buf[adress+1];

	 				 switch (buf[adress])
	 				{
	 					case OPTION_MESS_TYPE : 
	 					if ( buf[adress+2]==DHCP_OFFER && gest->state ==ST_DHCP_WAIT_OFFER)
	 					{
	 						gest->state=ST_DHCP_SEND_REQUEST;
	 						uint8_t i;
	 						for(i=0;i<6;i++)
	 							{
	                                gest->mac_routeur[i]=buf[ETH_SRC_MAC+i];
	                            }
	                            for(i=0;i<4;i++)
	 							{
	                                gest->ip_give[i]=buf[BOOTSTRAP_YOUR_IP+i];
	                            }
	 					}
	 						else if ( buf[adress+2]==DHCP_ACK && gest->state ==ST_DHCP_WAIT_ACK)
	 						{
	 							gest->state=ST_DHCP_OK;
	 							uint8_t i;
	 						for(i=0;i<6;i++)
	 							{
	                                gest->mac_routeur[i]=buf[ETH_SRC_MAC+i];
	                            }
	 						}
	 						for(i=0;i<4;i++)
	 							{
	                                gest->ip_give[i]=buf[BOOTSTRAP_YOUR_IP+i];
	                            }
	 						break;

	 					case OPTION_SERVER_ID :
	 					uint8_t i;
	 					for(i=0;i<4;i++)
	 						{gest->ip_routeur[i]=buf[adress+2+i];	}
	 					break;

	 					default : break;


	 				}
	 				adress=adress+2+len_adress;



	 			}
	                        return gest->state;
	 		}else return 0;
	 	}else return 0;

	}else return 0;
}

