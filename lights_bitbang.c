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

/* Initialize digital I/O port pins */
  DDRT = 0xFF; //enable PTT for output
  PTT_PTT0 = 0;
  PTT_PTT1 = 0;

}
	 		  			 		  		
/***********************************************************************
Main
***********************************************************************/
void main(void) {
  DisableInterrupts;
	initializations(); 		  			 		  		
	EnableInterrupts;

  PTT_PTT1 = 0; // start frame
  for (i=0;i<32;i++)
  {
    PTT_PTT0 = 1;
    PTT_PTT0 = 0;
  }
  
  for (j=0;j<30;j++)
  {
    PTT_PTT1 = 1; //  brightness
    for (i=0;i<8;i++)
    {
      PTT_PTT0 = 1;
      PTT_PTT0 = 0;
    }
    
    PTT_PTT1 = 0;
    for (i=0;i<8;i++) // blue
    {
      PTT_PTT0 = 1;
      PTT_PTT0 = 0;
    }
    
    PTT_PTT1 = 0;
    for (i=0;i<8;i++) // green
    {
      PTT_PTT0 = 1;
      PTT_PTT0 = 0;
    }
    
    PTT_PTT1 = 1;
    for (i=0;i<8;i++) // red
    {
      PTT_PTT0 = 1;
      PTT_PTT0 = 0;
    }

    PTT_PTT1 = 1; //  brightness
    for (i=0;i<8;i++)
    {
      PTT_PTT0 = 1;
      PTT_PTT0 = 0;
    }
    
    PTT_PTT1 = 1;
    for (i=0;i<8;i++) // blue
    {
      PTT_PTT0 = 1;
      PTT_PTT0 = 0;
    }
    
    PTT_PTT1 = 0;
    for (i=0;i<8;i++) // green
    {
      PTT_PTT0 = 1;
      PTT_PTT0 = 0;
    }
    
    PTT_PTT1 = 0;
    for (i=0;i<8;i++) // red
    {
      PTT_PTT0 = 1;
      PTT_PTT0 = 0;
    }
  }
  
  PTT_PTT1 = 0; // end frame
  for (j=0;j<4;j++)
  {
    for (i=0;i<8;i++)
    {
      PTT_PTT0 = 1;
      PTT_PTT0 = 0;
    }
  }
  
}/* do not leave main */
