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

/* ASCII character definitions */
#define CR 0x0D	// ASCII return character   

/* LCD COMMUNICATION BIT MASKS */
#define RS 0x04		// RS pin mask (PTT[2])
#define RW 0x08		// R/W pin mask (PTT[3])
#define LCDCLK 0x10	// LCD EN/CLK pin mask (PTT[4])

/* LCD INSTRUCTION CHARACTERS */
#define LCDON 0x0F	// LCD initialization command
#define LCDCLR 0x01	// LCD clear display command
#define TWOLINE 0x38	// LCD 2-line enable command
#define CURMOV 0xFE	// LCD cursor move instruction
#define LINE1 0x80	// LCD line 1 cursor position
#define LINE2 0xC0	// LCD line 2 cursor position

/* LED BIT MASKS */
#define GREEN 0x20
#define RED 0x40
#define YELLOW 0x80

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

/* Initialize SPI for baud rate of 6 Mbs
  DDRM   = 0xFF;
  SPICR1 = 0x50;
  SPICR2 = 0x00;
  SPIBR  = 0x01;
*/

/* Initialize digital I/O port pins */
  DDRT = 0xFF; //enable PTT for output
  PTT_PTT0 = 0;
  PTT_PTT1 = 0;

/* Initialize the LCD */  
//  lcdwait();      // wait for 2ms so that the LCD can wake up   
  
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
