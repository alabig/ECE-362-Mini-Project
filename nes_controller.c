// ***********************************************************************
//  ECE 362 - Final Project - Spring 2016
// ***********************************************************************

#include <hidef.h>           /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>

char a = 1;
char b = 1;
char sel = 1;
char start = 1;
char up = 1;
char down = 1;
char left = 1;
char right = 1;
	 	   		
// Initializations
 
void  initializations(void) {

// Set the PLL speed (bus clock = 24 MHz)

  		CLKSEL = CLKSEL & 0x80; // disengage PLL from system
  		PLLCTL = PLLCTL | 0x40; // turn on PLL
  		SYNR = 0x02;            // set PLL multiplier
  		REFDV = 0;              // set PLL divider
  		while (!(CRGFLG & 0x08)){  }
  		CLKSEL = CLKSEL | 0x80; // engage PLL
  
// Disable watchdog timer (COPCTL register)

      COPCTL = 0x40;    //COP off - RTI and COP stopped in BDM-mode

//  Initialize digital inputs

	    DDRAD = 0; 		//program port AD for input mode
      ATDDIEN = 0x01; //program PAD0 as digital inputs
         
//  Add additional port pin initializations here  (e.g., Other DDRs, Ports) 

      DDRT = 0xFF;  // program port T for output mode
      PTT_PTT0 = 0; // pulse signal
      PTT_PTT1 = 0; // latch signal
	  
//  Add RTI/interrupt initializations here

      CRGINT = 0x80;
      RTICTL = 0x71;  // approx 60Hz
}
	 		  			 		  		
 
// Main (non-terminating loop)
 
void main(void) {
	initializations(); 		  			 		  		
	EnableInterrupts;


  for(;;) {


  }/* loop forever */ 
}/* make sure that you never leave main */



// ***********************************************************************                       
// RTI interrupt service routine: rti_isr
// ***********************************************************************                       
 
interrupt 7 void RTI_ISR( void)
{
  CRGFLG = CRGFLG | 0x80; // set CRGFLG bit to clear RTI device flag

	PTT_PTT1 = 1; // latch data
	PTT_PTT1 = 0;
	a = PORTAD0_PTAD0;	// get a
		
	PTT_PTT0 = 1; // send pulse
	PTT_PTT0 = 0;
	b = PORTAD0_PTAD0;	// get b
	
	PTT_PTT0 = 1; // send pulse
	PTT_PTT0 = 0;
	sel = PORTAD0_PTAD0;	// get sel
	
	PTT_PTT0 = 1; // send pulse
	PTT_PTT0 = 0;
	start = PORTAD0_PTAD0;	// get start
	
	PTT_PTT0 = 1; // send pulse
	PTT_PTT0 = 0;
	up = PORTAD0_PTAD0;	// get up
	
	PTT_PTT0 = 1; // send pulse
	PTT_PTT0 = 0;
	down = PORTAD0_PTAD0;	// get down
	
	PTT_PTT0 = 1; // send pulse
	PTT_PTT0 = 0;
	left = PORTAD0_PTAD0;	// get left
	
	PTT_PTT0 = 1; // send pulse
	PTT_PTT0 = 0;
	right = PORTAD0_PTAD0;	// get right
		
	// Output debugging fun
	if (!b) PTT_PTT7 = 1;
	else PTT_PTT7 = 0;
	if (!a) PTT_PTT6 = 1;
	else PTT_PTT6 = 0;

}
