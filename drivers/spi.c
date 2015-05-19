#include "spi.h"



int writeSpi(unsigned char data){
  
	unsigned long timeout = 0x30000; // On défini le temps avant lequel on annule la transmission si elle se passe pas comme il faut
    U0DBUF = data; // On remplie le buffer.
    while(!(U0CSR & U0CSR_TX_BYTE)&& (--timeout!=0)) ;
      if(timeout==0) return -1; // On lance l'envoie de la donnée, si ça se passe mal on aura en retour un erreur
  //halWait(20); // Synschronisation
   while((U0CSR & U0CSR_ACTIVE));
}


int readSpi(unsigned char * data){
  
	unsigned long timeout = 0x30000;
  	U0DBUF=0x00; // On remplie le buffer pour générer l'horloge SCK
  	while(!(U0CSR & U0CSR_TX_BYTE)&& (--timeout!=0)) 
      	if(timeout==0) return -1;
  	U0CSR &= ~U0CSR_TX_BYTE;  // Wait for byte to be received from slave
  	while((U0CSR & U0CSR_ACTIVE)); // Synschronisation
    *data = U0DBUF; 
  
  	return 1;
        
}

void initSpi(void){
	// Set USART to SPI mode and Master mode
    U0CSR &= ~(0x80 | 0x20);
     
    U0BAUD = SPI_BAUD_M;
    U0GCR = (U0GCR & ~(U0GCR_BAUD_E))| SPI_BAUD_E | U0GCR_CPOL | U0GCR_CPHA | U0GCR_ORDER;
}


void halWait(unsigned char wait){
   unsigned long largeWait;

   if(wait == 0)
   {return;}

   largeWait = ((unsigned short) (wait << 7));
   largeWait += 59*wait;

   largeWait = (largeWait >> CLKSPD);
   while(largeWait--);

   return;
}

