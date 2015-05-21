#include "initCC1110.h"

void ioCfg(void){
        //PERCFG = 0X
	P0SEL  = 0x2C;	// Port0_(0,1,4,6,7) pins : GPIO
                        // Port0_(2,3,5)     pins : Periph
	P1SEL  = 0x00;	// Port1 pins : GPIO
	P2SEL  = 0x00;	// Port2 pins : GPIO
	
	P0DIR  = 0x10;	// Collector Port0 pin map
			 //		         
			 // P0_0     TMP_OUT	                In	 
			 // P0_3     SPI//MOSI                  Out	     
			 // P0_4     SPI//CS                    Out	 
			 // P0_5     SPI//CLCK                  Out	 
			
 	P1DIR  = 0x04;	// Collector Port1 pin map
 			// 
                        // P1_0      INTA (int accelerometre )	In
 			// P1_2	     TEMP_EN                    Out
 			
	P2DIR  = 0x04;	// Collector Port2 pin map
			 // P2_0	NC
			 // DD		Inout
			 // DC		Out
					
	P1IEN  = 0x01;	// Collecteur IRQ
                        //
                        // P1_2      Accelerometer
                        

        P0INP  = 0x01;	// pin input mode : pull-up/pull-down (P0_0 Tri state)
	P1INP  = 0x00;
	P2INP  = 0x00;
        
	P0IFG  = 0x00;	// clear interrupt
	P1IFG  = 0x00;
	P2IFG  = 0x00;
		        
	ADCCFG = 0x01;	// P0_0 : TMP_OUT is configured to an ADC input pin.
        ADCCON2= 0x30;	// ADC configuration  pour biopic 
      //  ADCCON2= 0xB0;	// ADC configuration  pour aykow 
        
        IEN2 |= 0x11; // Interrupt enable register    
   //     IEN0 |= 0x80; // Enable interruption
        
}

extern void uip_log(char *msg)
{
	char *msp = msg;
}

extern void httpd_appcall(void){
	int a = 2;
}
