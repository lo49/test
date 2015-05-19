/** ** ** 
file -> Drivers for ENC28j60.

** ** **/
#include "enc28j60.h"

#define MIN(a,b) (a)<(b) ? (a):(b)
volatile unsigned char Enc28j60Bank;
volatile unsigned short NextPacketPtr;
 
void delay_us(int t1)
{
	while(t1--);
}

void delay_ms(int t1)
{ 
	volatile int i; 
	while(t1--) 
	{
		i=100;
	}
}

/**** 
Fonction : enc28j60ReadOp .
Description : Permet d'aller récupérer une donnée dans les registres (buffer/contrôle ...)
******/
unsigned char enc28j60ReadOp(unsigned char op, unsigned char address)
{
	unsigned char dat1;
	// activate CS	
	P0_4 =0;
	// issue read command
	writeSpi(op | (address & ADDR_MASK));	
	readSpi(&dat1);
	// do dummy read if needed (for mac and mii, see datasheet page 29)
	if(address & 0x80) readSpi(&dat1);
	// release CS
	P0_4=1; // Release CS
	return(dat1);
}

/*********
Fonction : enc28j60WriteOp
Description : Permet d'aller écrire une opération qui se compose : 
- code de l'opération sur 3 bits 
- adresse du registre à accéder sur 5 bits 
- de la donnée sur 8 bits.
*****/
void enc28j60WriteOp(unsigned char op, unsigned char address, unsigned char mydat)
{
	P0_4=0; // Activate CS
	// issue write command
	writeSpi( op | (address & ADDR_MASK));
	// write data
	writeSpi(mydat);
	P0_4=1; // Release CS
}


/**** 
Fonction : enc28j60SetBank
Description : Définis le Bank dont on a besoin.
*****/
void enc28j60SetBank(unsigned char address)
{
	// set the bank (if needed)
	if((address & BANK_MASK) != Enc28j60Bank)
	{
		// set the bank
		enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, (ECON1_BSEL1|ECON1_BSEL0));
		enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, (address & BANK_MASK)>>5);
		Enc28j60Bank = (address & BANK_MASK);
	}
}


/**** 
Fonction : enc28j60Read
Description : fonction qui automatise la lecture des données inclue dans le composant
*****/
unsigned char enc28j60Read(unsigned char address)
{
	// select bank to read
	enc28j60SetBank(address);	
	// do the read
	return enc28j60ReadOp(ENC28J60_READ_CTRL_REG, address);
}



/******
Fonction : enc28j60Write
Description : fonction qui automatise l'écriture des données inclue dans le composant
*****/
void enc28j60Write(unsigned char address, unsigned char mydat)
{
	// select bank to write
	enc28j60SetBank(address);

	// do the write
	enc28j60WriteOp(ENC28J60_WRITE_CTRL_REG, address, mydat);
}

/*******
Fonction : enc28j60_read_phyreg
Description : permet à l'utilisateur d'allez lire les registre PHY du composant
***/


unsigned short enc28j60_read_phyreg(unsigned char address)
{
	unsigned short mydat;
	
	// set the PHY register address
	enc28j60Write(MIREGADR, address);
	enc28j60Write(MICMD, MICMD_MIIRD);
	
	// Loop to wait until the PHY register has been read through the MII
	// This requires 10.24us
	while( (enc28j60Read(MISTAT) & MISTAT_BUSY) );
	
	// Stop reading
	enc28j60Write(MICMD, MICMD_MIIRD);
	
	// Obtain results and return
	mydat = enc28j60Read ( MIRDL );
	mydat |= enc28j60Read ( MIRDH );

	return mydat;
}
/*******
Fonction : enc28j60PhyWrite
Description :  permet à l'utilisateur d'allez écrire sur les registre PHY du composant
***/void enc28j60PhyWrite(unsigned char address, unsigned short mydat)
{
	// set the PHY register address
	enc28j60Write(MIREGADR, address);
	// write the PHY data
	enc28j60Write(MIWRL, mydat );
	enc28j60Write(MIWRH, mydat >> 8);
	// wait until the PHY write completes
	while(enc28j60Read(MISTAT) & MISTAT_BUSY)
	{
		delay_us(15);
	}
}
/*******
Fonction : enc28j60ReadBuffer
Description :  permet à l'utilisateur d'allez lire le buffer
***/

void enc28j60ReadBuffer(unsigned short len, unsigned char* dat)
{
    // assert CS
    P0_4 = 0;
    // issue read command
	writeSpi(ENC28J60_READ_BUF_MEM);
   
    while(len--)
    {
       
		readSpi(dat++);
    }	
    *dat='\0';
    // release CS
   P0_4 = 1;
}

/*******
Fonction : enc28j60WriteBuffer
Description :  permet à l'utilisateur d'allez écrire sur le buffer
***/
void enc28j60WriteBuffer(unsigned short len, unsigned char* dat)
{
    // assert CS
	
	P0_4 = 0;
    
    // issue write command

	writeSpi(ENC28J60_WRITE_BUF_MEM);
  
    while(len--)
    {
        writeSpi(*dat++);
    }	
    // release CS
	P0_4 = 1;
}


#define ETHERNET_MIN_PACKET_LENGTH	0x3C
#define ETHERNET_HEADER_LENGTH		0x0E

#define IP_TCP_HEADER_LENGTH 40
#define TOTAL_HEADER_LENGTH (IP_TCP_HEADER_LENGTH+ETHERNET_HEADER_LENGTH)

/*******
Fonction : enc28j60PacketSend
Description :  permet à l'utilisateur d'envoyé un paquet
***/
void enc28j60PacketSend(unsigned short len, unsigned char* packet)
 {
    // see http://forum.mysensors.org/topic/536/
    // while (readOp(ENC28J60_READ_CTRL_REG, ECON1) & ECON1_TXRTS)
    if (enc28j60Read(EIR) & EIR_TXERIF) {
        enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRST);
        enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRST);
        enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, EIR, EIR_TXERIF);
    }
    enc28j60Write(EWRPTL, TXSTART_INIT);
    enc28j60Write(EWRPTH, TXSTART_INIT>>8);

    // Set the TXND pointer to correspond to the packet size given
	enc28j60Write(ETXNDL, (TXSTART_INIT+len)&0xFF);
	enc28j60Write(ETXNDH, (TXSTART_INIT+len)>>8);
    
    enc28j60WriteOp(ENC28J60_WRITE_BUF_MEM, 0, 0x00);
    enc28j60WriteBuffer(len, packet);
    enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);
}


/*******
Fonction : enc28j60PacketReceive
Description :  permet à l'utilisateur de recevoir un paquet
***/

unsigned short enc28j60PacketReceive(unsigned short maxlen, unsigned char* packet)
{
   	uint16_t rxstat;
	uint16_t len;
	// check if a packet has been received and buffered
	//if( !(enc28j60Read(EIR) & EIR_PKTIF) ){
        // The above does not work. See Rev. B4 Silicon Errata point 6.
	if( enc28j60Read(EPKTCNT) ==0 ){
		return(0);
        }

	// Set the read pointer to the start of the received packet
	enc28j60Write(ERDPTL, (NextPacketPtr));
	enc28j60Write(ERDPTH, (NextPacketPtr)>>8);
	// read the next packet pointer
	NextPacketPtr  = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
	NextPacketPtr |= enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0)<<8;
	// read the packet length (see datasheet page 43)
	len  = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
	len |= enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0)<<8;
        len-=4; //remove the CRC count
	// read the receive status (see datasheet page 43)
	rxstat  = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
	rxstat |= enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0)<<8;
	// limit retrieve length
        if (len>maxlen-1){
                len=maxlen-1;
        }
        // check CRC and symbol errors (see datasheet page 44, table 7-3):
        // The ERXFCON.CRCEN is set by default. Normally we should not
        // need to check this.
        if ((rxstat & 0x80)==0){
                // invalid
                len=0;
        }else{
                // copy the packet from the receive buffer
                enc28j60ReadBuffer(len, packet);
        }
	// Move the RX read pointer to the start of the next received packet
	// This frees the memory we just read out
	enc28j60Write(ERXRDPTL, (NextPacketPtr));
	enc28j60Write(ERXRDPTH, (NextPacketPtr)>>8);
	// decrement the packet counter indicate we are done with this packet
	enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);
	return(len);
}


/*******
Fonction : enc28j60clkout
Description :  Change l'Horloge du composant
***/
void enc28j60clkout(unsigned char clk)
{
        //setup clkout: 2 is 12.5MHz:
    enc28j60Write(ECOCON, clk & 0x7);
}

/*******
Fonction : dev_init
Description :  appel de l'enc28j60_init pour le dev.
***/
void dev_init(void)
{
    enc28j60_init();
}

/*******
Fonction : dev_send
Description :  Fonction qui va avec uip permet de simplifier à l'utilisateur
***/
void dev_send(void)
{
  // enc28j60PacketSend(uip_len, uip_buf);
}

/*******
Fonction : dev_poll
Description :  Fonction qui va avec uip permet de simplifier à l'utilisateur
***/
unsigned short dev_poll(void)
{
 // return enc28j60PacketReceive(UIP_BUFSIZE, uip_buf);
  
}

/*******
Fonction : enc28j60_init
Description :  Fonction d'initialisation.
***/
void enc28j60_init(void)
{

// perform system reset
	enc28j60WriteOp(ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET);
	delay_ms(5);
	// check CLKRDY bit to see if reset is complete
        // The CLKRDY does not work. See Rev. B4 Silicon Errata point. Just wait.
	//while(!(enc28j60Read(ESTAT) & ESTAT_CLKRDY));
	// do bank 0 stuff
	// initialize receive buffer
	// 16-bit transfers, must write low byte first
	// set receive buffer start address
	NextPacketPtr = RXSTART_INIT;
        // Rx start
	enc28j60Write(ERXSTL, RXSTART_INIT&0xFF);
	enc28j60Write(ERXSTH, RXSTART_INIT>>8);
	// set receive pointer address
	enc28j60Write(ERXRDPTL, RXSTART_INIT&0xFF);
	enc28j60Write(ERXRDPTH, RXSTART_INIT>>8);
	// RX end
	enc28j60Write(ERXNDL, RXSTOP_INIT&0xFF);
	enc28j60Write(ERXNDH, RXSTOP_INIT>>8);
	// TX start
	enc28j60Write(ETXSTL, TXSTART_INIT&0xFF);
	enc28j60Write(ETXSTH, TXSTART_INIT>>8);
	// TX end
	enc28j60Write(ETXNDL, TXSTOP_INIT&0xFF);
	enc28j60Write(ETXNDH, TXSTOP_INIT>>8);
	// do bank 1 stuff, packet filter:
    	
	//
    	// 
        //
        //
	// do bank 2 stuff
	// enable MAC receive
	enc28j60Write(MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);
	// bring MAC out of reset
	enc28j60Write(MACON2, 0x00);
	// enable automatic padding to 60bytes and CRC operations
	enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, MACON3, MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN);
	// set inter-frame gap (non-back-to-back)
	enc28j60Write(MAIPGL, 0x12);
	enc28j60Write(MAIPGH, 0x0C);
	// set inter-frame gap (back-to-back)
	enc28j60Write(MABBIPG, 0x12);
	// Set the maximum packet size which the controller will accept
        // Do not send packets longer than MAX_FRAMELEN:
	enc28j60Write(MAMXFLL, MAX_FRAMELEN&0xFF);
	enc28j60Write(MAMXFLH, MAX_FRAMELEN>>8);
	// do bank 3 stuff
	// write MAC address
	// NOTE: MAC address in ENC28J60 is byte-backward
	enc28j60Write(MAADR5, UIP_ETHADDR0);
	enc28j60Write(MAADR4, UIP_ETHADDR1);
	enc28j60Write(MAADR3, UIP_ETHADDR2);
	enc28j60Write(MAADR2, UIP_ETHADDR3);
	enc28j60Write(MAADR1, UIP_ETHADDR4);
	enc28j60Write(MAADR0, UIP_ETHADDR5);


	FiltrageMac(UNICAST); // Accept ARP for this moment.
	
	// no loopback of transmitted frames
	enc28j60PhyWrite(PHCON2, PHCON2_HDLDIS);
	// switch to bank 0
	enc28j60SetBank(ECON1);
	// enable interrutps
	enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE|EIE_PKTIE);
	// enable packet reception
	enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);

}

// read the revision of the chip:
uint8_t enc28j60getrev(void)
{
	return(enc28j60Read(EREVID));
}

// Reset the receive buffer : 
void resetReceive(void){
	// On mets à jour le pointeur de paquet : 
	NextPacketPtr = RXSTART_INIT;
	// On envoie la demande de Reset:
	enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXRST);
	delay_ms(5);
	enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_RXEN);
}

// A chaque appel de la fonction on vient filtrer les paquets en précisant le type de filtre que l'on souhaite
// TODO : Ajouter des filtres disponible. ICMP (UDP/TCP)
void FiltrageMac(uint8_t typeFiltre ){
	  
	switch(typeFiltre)
	{
		case BROADCAST:
		// For broadcast packets we allow only ARP packtets
        // All other packets should be unicast only for our mac (MAADR)
        //
        // The pattern to match on is therefore
        // Type     ETH.DST
        // ARP      BROADCAST
        // 06 08 -- ff ff ff ff ff ff -> ip checksum for theses bytes=f7f9
        // in binary these poitions are:11 0000 0011 1111
        // This is hex 303F->EPMM0=0x3f,EPMM1=0x30
		enc28j60Write(ERXFCON, ERXFCON_CRCEN|ERXFCON_PMEN);
		enc28j60Write(EPMM0, 0x3f);
		enc28j60Write(EPMM1, 0x30);

		enc28j60Write(EPMCSL, 0xf9);
		enc28j60Write(EPMCSH, 0xf7);
		break;
	case  UNICAST:
		// For Packet IP and our Mac Addr
        // All other packets should be unicast only for our mac (MAADR)
        //
        // The pattern to match on is therefore
        // Type     ETH.DST
        // IP      
        // 00 08 -- uip_ETHADDR(defs.h) -> ip checksum for theses bytes=C6CD
        // in binary these positions are:11 0000 0011 1111
        // This is hex 303F->EPMM0=0x3f,EPMM1=0x30

		enc28j60Write(ERXFCON, 0x00);		
		enc28j60Write(ERXFCON, ERXFCON_UCEN);
		break;
	case MAC_IP_UDP_FILTER: 
			// For Packet IP and our Mac Addr and UDP
        // All other packets should be unicast only for our mac (MAADR)
        //
        // The pattern to match on is therefore
        // Type     ETH.DST
        // IP      UNICAST
        // 00 08 -- uip_ETHADDR(defs.h) -> ip checksum for theses bytes=C6CD
        // in binary these poitions are:11 0000 0011 1111
        // This is hex 303F->EPMM0=0x3f,EPMM1=0x30
		enc28j60Write(ERXFCON, ERXFCON_CRCEN|ERXFCON_PMEN);
		enc28j60Write(EPMM0, 0x3f);
		enc28j60Write(EPMM1, 0x30);
		enc28j60Write(EPMM2, 0x80);

		enc28j60Write(EPMCSL, 0xB5);
		enc28j60Write(EPMCSH, 0xCD);
		break;

	case MAC_IP_ICMP_FILTER: 
			// For Packet IP and our Mac Addr and UDP
        // All other packets should be unicast only for our mac (MAADR)
        //
        // The pattern to match on is therefore
        // Type     ETH.DST
        // IP      UNICAST
        // 00 08 -- uip_ETHADDR(defs.h) -> ip checksum for theses bytes=C6CD
        // in binary these poitions are:11 0000 0011 1111
        // This is hex 303F->EPMM0=0x3f,EPMM1=0x30
		enc28j60Write(ERXFCON, ERXFCON_CRCEN|ERXFCON_PMEN);
		enc28j60Write(EPMM0, 0x3f);
		enc28j60Write(EPMM1, 0x30);
		enc28j60Write(EPMM2, 0x80);

		enc28j60Write(EPMCSL, 0xCD);
		enc28j60Write(EPMCSH, 0xC5);
		break;

	default:
		enc28j60Write(ERXFCON, 0x00);
		break;
	}
}

