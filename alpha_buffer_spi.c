#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#define MSGLEN 512  // message buffer size
#define DISPLEN 8   // 8 characters per display

void shiftout(char x);
void lcdwait(void);

char upperchar, lowerchar;
int i, j;
char timcnt, tick;
char scrolldisp, wrapchar;

// message + whitespace must be at least 8 characters
// more than 8 characters will automatically scroll
char alphamsg [MSGLEN] = "        TETRIS IS COMING";
//char alphamsg [MSGLEN] = "          WHEN YOU THINK ABOUT IT YOU CAN ACTUALLY SAY QUITE A LOT USING ONLY 8 DIGITS ON A VERY SMALL DISPLAY THAT JUST KEEP SCROLLING AND SCROLLING UNTIL YOU EVENTUALLY RUN OUT OF EMBEDDED MEMORY ON THE MICRO AND HAVE TO LIMIT THE NUMBER OF CHARACTERS THAT ARE INCLUDED IN THE STRING YOU WANT TO DISPLAY BUT RIGHT NOW IT JUST KEEPS GOING ONE CHARACTER AFTER ANOTHER OMG WILL IT EVER END WILL IT EVER END WILL IT EVER END WILL IT EVER END WILL IT EVER END WILL IT EVER END WILL IT EVER END WILL IT EVER END YES";

char dispmsg [MSGLEN];

void initializations(void)
{
  // Set the PLL speed (bus clock = 24 MHz)
  CLKSEL = CLKSEL & 0x80; // disengage PLL from system
  PLLCTL = PLLCTL | 0x40; // turn on PLL
  SYNR = 0x02;            // set PLL multiplier
  REFDV = 0;              // set PLL divider
  while (!(CRGFLG & 0x08)){  }
  CLKSEL = CLKSEL | 0x80; // engage PLL
  
  // Disable watchdog timer (COPCTL register)
  COPCTL = 0x40;    //COP off - RTI and COP stopped in BDM-mode
  
  //  Initialize Port T Output Pins
  DDRT = 0xFF;  // program port T for output mode
  PTT_PTT0 = 1; // latch signal
  
  /* Initialize TIM Ch 7 (TC7) for periodic interrupts every 100 ms */
  TSCR1 = 0x80; // enable timer subsystem
  TIOS  = 0x80; // set channel 7 for output compare
  TSCR2 = 0x0C; // set appropriate pre-scale factor and enable counter reset after OC7
  TC7   = 15000;// set up channel 7 to generate 100 ms interrupt rate
  TIE   = 0x80; // enable TIM Ch 7 interrupts
  
  /* Initialize SPI for baud rate of 6 Mbs */
  DDRM   = 0xFF;
  SPICR1 = 0x54;  // use SCLK falling edge (default, should probably make sure it won't lead to failure)
  SPICR2 = 0x00;
  SPIBR  = 0x01;
  
  // wake up LCD
  lcdwait();
  lcdwait();
  lcdwait();
  
  // global initializations
  timcnt = 0;
  tick = 0;
  scrolldisp = 0;
  
}

void updatealphadisp(void) // update alpha LED display
{
  scrolldisp = 0;
  i = 0;
  PTT_PTT0 = 0;  // open latch
  
  while (dispmsg[i] && i < DISPLEN)
  {  
    switch (dispmsg[i++])
    {
      case 'A': upperchar = 0x00;
                lowerchar = 0xF7;
                break;
      case 'B': upperchar = 0x00;
                lowerchar = 0xBF;
                break;
      case 'C': upperchar = 0x00;
                lowerchar = 0x39;
                break;
      case 'D': upperchar = 0x12;
                lowerchar = 0x0F;
                break;
      case 'E': upperchar = 0x00;
                lowerchar = 0x79;
                break;
      case 'F': upperchar = 0x00;
                lowerchar = 0x71;
                break;
      case 'G': upperchar = 0x00;
                lowerchar = 0xBD;
                break;
      case 'H': upperchar = 0x00;
                lowerchar = 0xF6;
                break;
      case 'I': upperchar = 0x12;
                lowerchar = 0x09;
                break;
      case 'J': upperchar = 0x00;
                lowerchar = 0x1E;
                break;
      case 'K': upperchar = 0x0C;
                lowerchar = 0x70;
                break;
      case 'L': upperchar = 0x00;
                lowerchar = 0x38;
                break;
      case 'M': upperchar = 0x15;
                lowerchar = 0x36;
                break;
      case 'N': upperchar = 0x09;
                lowerchar = 0x36;
                break;
      case 'O': upperchar = 0x00;
                lowerchar = 0x3F;
                break;
      case 'P': upperchar = 0x00;
                lowerchar = 0xF3;
                break;
      case 'Q': upperchar = 0x08;
                lowerchar = 0x3F;
                break;
      case 'R': upperchar = 0x08;
                lowerchar = 0xF3;
                break;
      case 'S': upperchar = 0x09;
                lowerchar = 0x09;
                break;
      case 'T': upperchar = 0x12;
                lowerchar = 0x01;
                break;
      case 'U': upperchar = 0x00;
                lowerchar = 0x3E;
                break;
      case 'V': upperchar = 0x24;
                lowerchar = 0x30;
                break;
      case 'W': upperchar = 0x2A;
                lowerchar = 0x36;
                break;
      case 'X': upperchar = 0x2D;
                lowerchar = 0x00;
                break;
      case 'Y': upperchar = 0x15;
                lowerchar = 0x00;
                break;
      case 'Z': upperchar = 0x24;
                lowerchar = 0x09;
                break;
                
      case '1': upperchar = 0x04;
                lowerchar = 0x06;
                break;
      case '2': upperchar = 0x00;
                lowerchar = 0xDB;
                break;
      case '3': upperchar = 0x00;
                lowerchar = 0xCF;
                break;
      case '4': upperchar = 0x00;
                lowerchar = 0xE6;
                break;
      case '5': upperchar = 0x00;
                lowerchar = 0xED;
                break;
      case '6': upperchar = 0x00;
                lowerchar = 0xFD;
                break;
      case '7': upperchar = 0x00;
                lowerchar = 0x07;
                break;
      case '8': upperchar = 0x00;
                lowerchar = 0xFF;
                break;
      case '9': upperchar = 0x00;
                lowerchar = 0xEF;
                break;
      case '0': upperchar = 0x24;
                lowerchar = 0x3F;
                break;
      default:  upperchar = 0x00;
                lowerchar = 0x00;
                break;
    }
  
    shiftout(upperchar);  // shift in single character
    shiftout(lowerchar);
    
  }
  
  PTT_PTT0 = 1; // latch character data
  
  if (i == DISPLEN && dispmsg[i]) scrolldisp = 1;  // automatically scroll messages longer than display length

}

void main(void) {
  /* put your own code here */
 	EnableInterrupts;
  initializations();
  
  for (i = 0; alphamsg[i]; i++) dispmsg[i] = alphamsg[i];  
  dispmsg[i] = '\0';
  updatealphadisp();  // initially display message
  
  for(;;) {
    
    if (scrolldisp && tick) // scroll left with clock
    {
      scrolldisp = 0;
      tick = 0;
      wrapchar = dispmsg[0];
      for (i = 1; dispmsg[i]; i++) dispmsg[i-1] = dispmsg[i];
      i--;  // adjust for shifted end index
      dispmsg[i++] = wrapchar;
      dispmsg[i] = '\0';
      updatealphadisp();
    }
    
  _FEED_COP(); /* feeds the dog */
  }/* loop forever */
  /* please make sure that you never leave main */
}

/*
***********************************************************************
  TIM interrupt service routine
***********************************************************************
*/

interrupt 15 void TIM_ISR(void)
{
 	TFLG1 = TFLG1 | 0x80; // clear TIM CH 7 interrupt flag 
 	
 	timcnt++;
 	if (timcnt == 10)
 	{
 	  timcnt = 0;
 	  tick = 1;
 	}
}

/*
***********************************************************************
  lcdwait: Delay for approx 2 ms
***********************************************************************
*/
void lcdwait()
{
  int m = 8;
  int n = 6000;
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
  lcdwait();            // wait for (at least <>) 30 cycles for SPI data to shift out    
}
