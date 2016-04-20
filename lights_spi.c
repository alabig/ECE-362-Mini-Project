/*
***********************************************************************
 ECE 362 - Final Project - Spring 2016
***********************************************************************
*/

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>

/* All funtions after main should be initialized here */
void shiftout(char x);
void lcdwait(void);

/* Variable declarations */  	   			 		  			 		       
char i = 0;
char j = 0;

/***********************************************************************
Initializations
***********************************************************************/
void  initializations(void) {

/* Set the PLL speed (bus clock = 24 MHz) */
  CLKSEL = CLKSEL & 0x80; // disengage PLL from system
  PLLCTL = PLLCTL | 0x40; // turn on PLL
  SYNR = 0x02;            // set PLL multiplier
  REFDV = 0;              // set PLL divider
  while (!(CRGFLG & 0x08)){  }
  CLKSEL = CLKSEL | 0x80; // engage PLL

/* Disable watchdog timer (COPCTL register) */
  COPCTL = 0x40;   //COP off, RTI and COP stopped in BDM-mode

/* Initialize SPI for baud rate of 6 Mbs */
  DDRM   = 0xFF;
  SPICR1 = 0x50;
  SPICR2 = 0x00;
  SPIBR  = 0x01;

}
	 		  			 		  		
/***********************************************************************
Main
***********************************************************************/
void main(void) {
  DisableInterrupts;
	initializations(); 		  			 		  		
	EnableInterrupts;

  lcdwait();           
  
  // start frame
  shiftout(0x00);
  shiftout(0x00);
  shiftout(0x00);
  shiftout(0x00);
  
  // LED frame
  shiftout(0xFF);  // brightness
  shiftout(0xFF);  // blue
  shiftout(0x00);  // green
  shiftout(0x00);  // red
    
  for (i=0;i<59;i++)  // blank frames
  {
    shiftout(0xE0);
    shiftout(0x00);
    shiftout(0x00);
    shiftout(0x00);
  }
    
  // end frame for 60 LEDs
  shiftout(0x00);
  shiftout(0x00);
  shiftout(0x00);
  shiftout(0x00);
    
}/* do not leave main */


/***********************************************************************
  shiftout: Transmits the character x to external shift 
            register using the SPI.  It should shift MSB first.  
             
            MOSI = PM[4]
            SCK  = PM[5]
***********************************************************************/

void shiftout(char x)

{
  while(!SPISR_SPTEF) {}// read the SPTEF bit, continue if bit is 1
  SPIDR = x;            // write data to SPI data register
  //lcdwait();            // wait for (at least <>) 30 cycles for SPI data to shift out  
  
}


/***********************************************************************
  lcdwait: Delay for approx 2 ms
***********************************************************************/
void lcdwait()
{
  int m = 5;
  int n = 5000;
  while(m > 0)
  {
    while(n > 0)
    {
      n--;
      asm{
      nop;
      }
    }
    m--;
  }
}
