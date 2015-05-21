#include "initCC1110.h"
#include "spi.h"
#include "enc28j60.h"

#include "udp.h"
#include "defs.h"

#define LOIC

//#define FREE 
#ifdef CORENTIN
 void
 main(void)
 {
  uint8_t mymac[6] = {UIP_ETHADDR0,UIP_ETHADDR1,UIP_ETHADDR2,UIP_ETHADDR3,UIP_ETHADDR4,UIP_ETHADDR5};
  uint8_t myip[4] = {0x00,0x00,0x00,0x00};
  // Initialisation : 
   ioCfg();
   initSpi();
   dev_init();
   init_ip_arp_udp_tcp(mymac, myip);
    dhcp_discover(buffer);

    do
    {
        //unsigned short enc28j60PacketReceive;(unsigned short maxlen, unsigned char* packet);
        
        uint8_t len=enc28j60PacketReceive(BUFFER_SIZE,buffer);
        if(len==0) ;
        else {} 

    }while(1);
  }
#endif

#ifdef LOIC
void main(void)
{
  uint8_t mymac[6] = {UIP_ETHADDR0,UIP_ETHADDR1,UIP_ETHADDR2,UIP_ETHADDR3,UIP_ETHADDR4,UIP_ETHADDR5};
  ioCfg();
  initSpi();
  dev_init();
  // On cherche à obtenir notre addresse par le biais du routeur :
  DHCP ip = dhcp(buffer);
  TCP conn = tcp(buffer,ip);
}

#endif
  

 /*---------------------------------------------------------------------------*/