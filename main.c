#include "initCC1110.h"
#include "spi.h"
#include "enc28j60.h"

#include "defs.h"
//#define FREE 

 void
 main(void)
 {
  // Initialisation : 
   ioCfg();
   initSpi();
   dev_init();

  }
  
  


 /*---------------------------------------------------------------------------*/