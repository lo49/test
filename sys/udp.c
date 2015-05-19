/* File : udp.c 
Author : Aykow
date : 13 mai 2015

*/

// Gère la demande de découverte
void dhcp_discover(uint8_t *buf){ 

	uint8_t dst_mac[6]={0xff,0xff,0xff,0xff,0xff,0xff} ;
	make_eth_ip_new(buf, dst_mac);

	// IP protocole

	uint8_t dst_ip[4]={255,255,255,255};
	make_ip_udp_new(buf, 350, dst_ip);

make_udp_header(68,67,308);


}


void make_udp_header(uint16_t source_port, uint16_t dest_port, uint16_t longueur)
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

void make_booststrap_protocol (uint8_t type_mess, uint16_t bootp_flags)
{

	buf[BOOTSTRAP_MESS_TYPE]= type_mess;
	buf[BOOTSTRAP_HDW_TYPE]= 0x01;
	buf[BOOTSTRAP_ADDR_LEN]= 0x06;
	buf[BOOTSTRAP_HOPS]= 0x00;
	buf[BOOTSTRAP_TR_ID]= 0x99;
	buf[BOOTSTRAP_TR_ID]= 0x994a6119;
	buf[BOOTSTRAP_SC_EL]= 0x0000;
	buf[BOOTSTRAP_FLAGS]= bootp_flags>>8 & 0xff;
	buf[BOOTSTRAP_FLAGS]=bootp_flags+1 & 0xff;
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


