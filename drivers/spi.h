#ifndef SPI_H
#define SPI_H
#include <ioCC1110.h>


#define CLKSPD  (CLKCON & 0x07)


//typedef unsigned char       uint8_t;
//typedef unsigned long       uint32_t;
//typedef unsigned short      uint16_t;


#define RD 0x80//read command
#define WR 0x00//write command
#define MB 0x40//Multiple reading command

#define SPI_BAUD_M                        117
#define SPI_BAUD_E                        8
#define U0GCR_CPOL                        0x00 // low when idle clock
#define U0GCR_CPHA                        0x00
#define U0GCR_ORDER                       0x20
#define U0GCR_BAUD_E                      0x1F

#define PERCFG_U1CFG                      0x02
#define PERCFG_U0CFG                      0x01

#define P2DIR_PRIP0                       0xC0
#define P2DIR_PRIP0_0                     (0x00 << 6)
#define BIT0                              0x01
#define BIT1                              0x02
#define BIT2                              0x04
#define BIT3                              0x08
#define BIT4                              0x10
#define BIT5                              0x20
#define BIT6                              0x40
#define BIT7                              0x80
    
#define U0CSR_MODE                        0x80
#define U0CSR_RE                          0x40
#define U0CSR_SLAVE                       0x20
#define U0CSR_FE                          0x10
#define U0CSR_ERR                         0x08
#define U0CSR_RX_BYTE                     0x04
#define U0CSR_TX_BYTE                     0x02
#define U0CSR_ACTIVE                      0x01


#define	DEVID		0x00	//Device ID Register
#define THRESH_TAP	0x1D	//Tap Threshold
#define	OFSX		0x1E	//X-axis offset
#define	OFSY		0x1F	//Y-axis offset
#define	OFSZ		0x20	//Z-axis offset
#define	DURATION	0x21	//Tap Duration
#define	LATENT		0x22	//Tap latency
#define	WINDOW		0x23	//Tap window
#define	THRESH_ACT	0x24	//Activity Threshold
#define	THRESH_INACT	0x25	//Inactivity Threshold
#define	TIME_INACT	0x26	//Inactivity Time
#define	ACT_INACT_CTL	0x27	//Axis enable control for activity and inactivity detection
#define	THRESH_FF	0x28	//free-fall threshold
#define	TIME_FF		0x29	//Free-Fall Time
#define	TAP_AXES	0x2A	//Axis control for tap/double tap
#define ACT_TAP_STATUS	0x2B	//Source of tap/double tap
#define	BW_RATE		0x2C	//Data rate and power mode control
#define POWER_CTL	0x2D	//Power Control Register
#define	INT_ENABLE1	0x2E	//Interrupt Enable Control
#define	INT_MAP		0x2F	//Interrupt Mapping Control
#define	INT_SOURCE	0x30	//Source of interrupts
#define	DATA_FORMAT	0x31	//Data format control
#define DATAX0		0x32	//X-Axis Data 0
#define DATAX1		0x33	//X-Axis Data 1
#define DATAY0		0x34	//Y-Axis Data 0
#define DATAY1		0x35	//Y-Axis Data 1
#define DATAZ0		0x36	//Z-Axis Data 0
#define DATAZ1		0x37	//Z-Axis Data 1
#define	FIFO_CTL	0x38	//FIFO control
#define	FIFO_STATUS	0x39	//FIFO status


int writeSpi(unsigned char data);
int readSpi(unsigned char * data);
void initSpi(void);
void halWait(unsigned char wait);



#endif