/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher 
 * Copyright: GPL V2
 *
 * IP/ARP/UDP/TCP functions
 *
 * 
 *********************************************/
 /*********************************************
 * Modified: nuelectronics.com -- Ethershield for Arduino
 *********************************************/
//@{
#ifndef IP_ARP_UDP_TCP_H
#define IP_ARP_UDP_TCP_H

//static uint8_t ipaddr[4];
static uint8_t ipserverDHCP[4];
static uint8_t broadaddr[4];
static uint8_t macaddr[6];

// you must call this function once before you use any of the other functions:
extern void init_ip_arp_udp_tcp(uint8_t *mymac, uint8_t *myip);

extern uint16_t checksum(uint8_t *buf, uint16_t len,uint8_t type);
//
extern uint8_t eth_type_is_arp_and_my_ip(uint8_t *buf,uint16_t len);
extern uint8_t eth_type_is_ip_and_my_ip(uint8_t *buf,uint16_t len);
extern uint8_t eth_type_is_ip_and_protocol(uint8_t *buf,uint16_t len,uint8_t protocol);
extern uint8_t eth_type_is_ip_and_my_mac(uint8_t * buf,uint16_t len);
extern void make_arp_answer_from_request(uint8_t *buf);
extern void make_echo_reply_from_request(uint8_t *buf,uint16_t len);
extern void make_echo_request(uint8_t* buf,uint8_t * dst_ip,uint8_t * dest_mac);
extern void make_udp_reply_from_request(uint8_t *buf,char *data,uint8_t datalen,uint16_t port);
extern void make_eth_ip_new(uint8_t *buf, uint8_t* dst_mac);

extern void make_tcp_synack_from_syn(uint8_t *buf);
extern void make_tcp_syn(uint8_t *buf,uint8_t * mac_routeur,uint8_t * ip_distant,uint16_t dst_port,uint16_t src_port);
extern void init_len_info(uint8_t *buf);
extern uint16_t get_tcp_data_pointer(void);
extern uint16_t fill_tcp_data_p(uint8_t *buf,uint16_t pos, const char *progmem_s);
extern uint16_t fill_tcp_data(uint8_t *buf,uint16_t pos, const char *s);
extern void fill_ip_hdr_checksum(uint8_t *buf);
extern void make_tcp_ack_from_any(uint8_t *buf);
extern void make_tcp_ack_with_data(uint8_t *buf,uint16_t dlen);
extern void make_arp_request(uint8_t *buf, uint8_t *server_ip);
extern uint8_t arp_packet_is_myreply_arp ( uint8_t *buf );
extern void tcp_client_send_packet(uint8_t *buf,uint16_t dest_port, uint16_t src_port, uint8_t flags, uint32_t ack_num, 
        uint32_t seq_num, uint16_t next_ack_num, uint16_t dlength, uint8_t *dest_mac, uint8_t *dest_ip);
extern uint16_t tcp_get_dlength ( uint8_t *buf );

// UDP
extern uint16_t get_udp_port_src(uint8_t *buf);
extern uint16_t get_udp_port_dst(uint8_t *buf);
extern uint16_t udp_get_dlength ( uint8_t *buf );

extern void ping(uint8_t *buf, uint8_t *dst_mac, uint8_t *dest_ip);


static uint16_t ip_identifier = 1;

#endif /* IP_ARP_UDP_TCP_H */
//@}
