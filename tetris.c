#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <stdlib.h>

#define TETRIS 4

#define NUMROWS 23  // 3 extra rows to allow movement as piece appears
#define NUMCOLS 10

#define NUMPIECES 7
#define SIZE 4

void shiftout(char x);

char x, y, z, rot;
char row, col, fullrow, fullrow;
char n, i;

char tick, timcnt, level;
char roundover, gameover, paused;

char prevup, prevdown, prevleft, prevright, preva, prevb, prevsel, prevstart;
char up, down, left, right, a, b, sel, start;
unsigned char red, green, blue; // RGB values (0-255)

char flash, flashdelay;
char rclear[5] = {-1,-1,-1,-1,-1};  // rows to clear
char numrclear, tchain;

char gameboard[NUMROWS][NUMCOLS];
char currentpiece[4][4];

int score, highscore;

char block[7][4][4][4] = {
{ // iblock([0])
  {  
    {0,0,0,0},
    {0,0,0,0},
    {1,1,1,1},
    {0,0,0,0}
  },
  {
    {0,1,0,0},
    {0,1,0,0},
    {0,1,0,0},
    {0,1,0,0}
  },
  {
    {0,0,0,0},
    {1,1,1,1},
    {0,0,0,0},
    {0,0,0,0}
  },
  {
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0}
  }
},
{ // tblock([1])
  {  
    {0,2,0},
    {2,2,2},
    {0,0,0}
  },
  {
    {0,2,0},
    {0,2,2},
    {0,2,0}
  },
  {
    {0,0,0},
    {2,2,2},
    {0,2,0}
  },
  {
    {0,2,0},
    {2,2,0},
    {0,2,0}
  }
},
{ // zblock([2])
  {  
    {3,3,0},
    {0,3,3},
    {0,0,0}
  },
  {
    {0,3,0},
    {3,3,0},
    {3,0,0}
  },
  {
    {0,0,0},
    {3,3,0},
    {0,3,3}
  },
  {
    {0,0,3},
    {0,3,3},
    {0,3,0}
  }
},
{ // sblock([3])
  {  
    {0,4,4},
    {4,4,0},
    {0,0,0}
  },
  {
    {0,4,0},
    {0,4,4},
    {0,0,4}
  },
  {
    {0,0,0},
    {0,4,4},
    {4,4,0},
  },
  {
    {4,0,0},
    {4,4,0},
    {0,4,0}
  }
},
{ // jblock([4])
  {  
    {5,0,0},
    {5,5,5},
    {0,0,0}
  },
  {
    {0,5,5},
    {0,5,0},
    {0,5,0}
  },
  {
    {0,0,0},
    {5,5,5},
    {0,0,5}
  },
  {
    {0,5,0},
    {0,5,0},
    {5,5,0}
  }
},
{ // lblock([5])
  {  
    {0,0,6},
    {6,6,6},
    {0,0,0}
  },
  {
    {0,6,0},
    {0,6,0},
    {0,6,6}
  },
  {
    {0,0,0},
    {6,6,6},
    {6,0,0}
  },
  {
    {6,6,0},
    {0,6,0},
    {0,6,0}
  }
},
{ // oblock([6])
  {
    {7,7,0},
    {7,7,0},
    {0,0,0}
  },
  {
    {0,7,7},
    {0,7,7},
    {0,0,0}
  },
  {
    {0,0,0},
    {0,7,7},
    {0,7,7}
  },
  {  
    {0,0,0},
    {7,7,0},
    {7,7,0}
  }
}
};

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
  
  
  //  Initialize digital inputs
	DDRAD = 0; 		//program port AD for input mode
  ATDDIEN = 0x01; //program PAD0 as digital input from NES controller
  
  //  Add RTI/interrupt initializations here
  CRGINT = 0x80;
  RTICTL = 0x71;  // approx 60Hz
  
  //  Add additional port pin initializations here  (e.g., Other DDRs, Ports) 
  
  //  NES controller
  DDRT = 0xFF;  // program port T for output mode
  PTT_PTT0 = 0; // pulse signal
  PTT_PTT1 = 0; // latch signal
  
  /* Initialize TIM Ch 7 (TC7) for periodic interrupts every 100 ms */
  TSCR1 = 0x80; // enable timer subsystem
  TIOS  = 0x80; // set channel 7 for output compare
  TSCR2 = 0x0C; // set appropriate pre-scale factor and enable counter reset after OC7
  TC7   = 15000;// set up channel 7 to generate 100 ms interrupt rate
  TIE   = 0x80; // enable TIM Ch 7 interrupts
  
  /* Initialize SPI for baud rate of 6 Mbs */
  DDRM   = 0xFF;
  SPICR1 = 0x50;
  SPICR2 = 0x00;
  SPIBR  = 0x01;
  
  //initialize gameboard
  for (y = 0;y < NUMROWS;y++)
    for (x = 0;x < NUMCOLS;x++) gameboard[y][x] = 0;
    
  // global initializations
  paused = 0;
  rot = 0;
  roundover = 1;
  fullrow = 0;
  tick = 0;
  timcnt = 0;
  level = 0;
  gameover = 0;
  prevup = 1;
  prevdown = 1;
  prevleft = 1;
  prevright = 1;
  preva = 1;
  prevb = 1;
  prevsel = 1;
  prevstart = 1;
  up = 0;
  down = 0;
  left = 0;
  right = 0;
  a = 0;
  b = 0;
  sel = 0;
  start = 0;
  flash = 0;
  numrclear = 0;
  tchain = 0;
  score = 0;
  
  // load highscore  
}

int checkcollision(void)
{
  int collision = 0;
  
  for (y = 0; y < SIZE; y++)
    for (x = 0; x < SIZE; x++)
      if (currentpiece[SIZE - 1 - y][x])
        if ((col + x < 0) || (col + x > 9) || (row + y < 0) || (gameboard[row + y][col + x])) collision = 1;
    
  return collision;
}

void clearpiece(void)
{
  for (y = 0; y < SIZE; y++)
    for (x = 0; x < SIZE; x++)
      if (currentpiece[SIZE - 1 - y][x]) gameboard[row + y][col + x] = 0;
}

void setpiece(void)
{
  for (y = 0; y < SIZE; y++)
    for (x = 0; x < SIZE; x++)
      if (currentpiece[SIZE - 1 - y][x]) gameboard[row + y][col + x] = currentpiece[SIZE - 1 - y][x];
}

void updatecurrentpiece(char dx)  // derpy update routine
{
  //for (z = 0;z < 4;z++)
    for (y = 0;y < 4;y++)
      for (x = 0;x < 4;x++)
        currentpiece[y][x] = block[n][dx][y][x];
}

void updatedisp(void) // update LED data
{
  // start frame
  shiftout(0x00);
  shiftout(0x00);
  shiftout(0x00);
  shiftout(0x00);
  
  x = 0;
  y = -1;
  while (x < NUMCOLS)
  {
    if (x % 2) y--;
    else y++;
      
    for (i = 0;i < (NUMROWS - 3); i++)
    { 
      if (flash && ((y == rclear[0]) || (y == rclear[1]) || (y == rclear[2]) || (y == rclear[3])))
      {
        red = 255;    // white
        green = 255;
        blue = 255;
      }
      else    
      switch (gameboard[y][x])
      {
        case 0: red = 0;    // blank
                green = 0;
                blue = 0;
                break;
        case 1: red = 0;    // cyan (i-block)
                green = 240;
                blue = 240;
                break;
        case 2: red = 161;  // purple (t-block)
                green = 0;
                blue = 240;
                break;
        case 3: red = 255;  // red (z-block)
                green = 0;
                blue = 0;
                break;
        case 4: red = 0;    // green (s-block)
                green = 255;
                blue = 0;
                break;
        case 5: red = 0;    // blue (j-block)
                green = 0;
                blue = 255;
                break;
        case 6: red = 240;  // orange (l-block)
                green = 161;
                blue = 0;
                break;
        case 7: red = 240;  // yellow (o-block)
                green = 240;
                blue = 0;
                break;
        default:red = 255;  // white by default (it's not a racial thing)
                green = 255;
                blue = 255;
      }
      
      // LED frame
      shiftout(0xFF); // brightness
      shiftout(blue); // blue
      shiftout(green);// green
      shiftout(red);  // red      
      
      if (x % 2) y--;
      else y++;
    }
    x++;
  }
    
  // end frame for 200 LEDs
  for (i = 0; i < 13; i++) shiftout(0x00);
}

void flashrows(void)  // flashing effect for clear rows
{
  for (z = 0; z < 4; z++)
  { 
    updatedisp();
    flash = 1;
    updatedisp();
    flash = 0;
    
    flashdelay = 1;
    while (flashdelay) {} // flash delay
  }
}

void clearrows(void)
{
  for (y = 0; y < NUMROWS; y++)
  {
    fullrow = 1;
    for (x = 0; x < NUMCOLS; x++)
      if (gameboard[y][x] == 0) fullrow = 0;
    if (fullrow)
    {
      row = y;
      while (row++ < NUMROWS)
        for (x = 0; x < NUMCOLS; x++)
          gameboard[row][x] = gameboard[row + 1][x];
    }
  }
}

void checkrclear(void)
{
  numrclear = 0;
  for (y = 0, i = 0; y < SIZE; y++) // check for full rows
  {
    fullrow = 1;
    if ((row + y) >= 0)
    { 
      for (x = 0; x < NUMCOLS; x++)
        if (gameboard[row + y][x] == 0) fullrow = 0;
      if (fullrow)
      {
        rclear[i++] = row + y;
        numrclear++;
      }
    }
  }
}

void main(void) {
  /* put your own code here */
 	EnableInterrupts;

  for(;;) {

  while (!gameover)
  {    
    if (tick)
    {
      tick = 0;
      
      if (roundover)
      {
        roundover = 0;        
        score += 10 * (level+1);
        
        checkrclear();
        if (numrclear)
        {
          if (numrclear == TETRIS)
          {
            if (tchain) score += 1200;
            else score += 800;
            tchain = 1;
          }
          else
          { 
            score += 100 * numrclear;
            tchain = 0;
          }
          
          flashrows();
          clearrows();
          for (i = 0; i < SIZE; i++) rclear[i] = -1;
          numrclear = 0;
        }
        
        n = (char)(rand() % (NUMPIECES)); // set new piece as currentpiece
        updatecurrentpiece(0);
        
        // reset row and col for new piece
        row = NUMROWS - 4;
        col = NUMCOLS - 7;
        
        if (checkcollision()) gameover = 1;  // unable to initially place new piece, game over
        else updatedisp();  // update board with initial placement of new piece
      }
      else
      {
        clearpiece();
        row--;
        if (checkcollision())
        {
          row++;
          setpiece();
          roundover = 1;
        }
        else
        {
          setpiece();
          updatedisp();
        }
      }
    }
    
    if (!paused)
    {
      
      if (left)
      {
        left = 0;
        clearpiece();
        col--;
        if (checkcollision())
        {
          col++;
          setpiece();
        }
        else
        {
          setpiece();
          updatedisp();
        }
      }
      else if (right)
      {
        right = 0;
        clearpiece();
        col++;
        if (checkcollision())
        {
          col--;
          setpiece();
        }
        else
        {
          setpiece();
          updatedisp();
        }
      }
      else if (a) // clockwise
      {
        a = 0;
        clearpiece();
        rot = (char)(++rot % 4);  //update current piece
        updatecurrentpiece(rot);
        if (checkcollision()) //undo update
        {
          rot = (char)(--rot % 4);
          updatecurrentpiece(rot);
        }
        setpiece();
        updatedisp();
      }
      else if (b) // counterclockwise
      {
        b = 0;
        clearpiece();
        rot = (char)(--rot % 4);  //update current piece
        updatecurrentpiece(rot);
        if (checkcollision()) //undo update
        {
          rot = (char)(++rot % 4);
          updatecurrentpiece(rot);
        }
        setpiece();
        updatedisp();
      }
      else if (down)
      {
        down = 0;
        clearpiece();
        row--;
        if (checkcollision())
        {
          row++;
          setpiece();
          roundover = 1;
        }
        else
        {
          setpiece();
          updatedisp();
        }
      }
      else if (up)  // drop piece
      {
        up = 0;
        clearpiece();
        do row--;
        while (!checkcollision());
        row++;
        setpiece();
        updatedisp();
        roundover = 1;
      }
    }
    if (start) // toggle pause
    {
      start = 0;
      if (paused) paused = 0;
      else paused = 1;
    }
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
 	
 	if (!paused)
 	{ 	  
   	timcnt++;
   	if (timcnt == (10 - level))
   	{
   	  timcnt = 0;
   	  tick = 1;
   	}
   	if (flashdelay) flashdelay = 0;
 	}
}

// ***********************************************************************                       
// RTI interrupt service routine: rti_isr
// ***********************************************************************                       
 
interrupt 7 void RTI_ISR( void)
{
  CRGFLG = CRGFLG | 0x80; // set CRGFLG bit to clear RTI device flag

	PTT_PTT1 = 1; // latch data
	PTT_PTT1 = 0;
	
	if (!PORTAD0_PTAD0)	// get a
	  if (preva) a = 1;
	preva = PORTAD0_PTAD0;
		
	PTT_PTT0 = 1; // send pulse
	PTT_PTT0 = 0;
	
	if (!PORTAD0_PTAD0)	// get b
	  if (prevb) b = 1;
	prevb = PORTAD0_PTAD0;
	
	PTT_PTT0 = 1; // send pulse
	PTT_PTT0 = 0;
	
	if (!PORTAD0_PTAD0)	// get sel
	  if (prevsel) sel = 1;
	prevsel = PORTAD0_PTAD0;
	
	PTT_PTT0 = 1; // send pulse
	PTT_PTT0 = 0;
	
	if (!PORTAD0_PTAD0)	// get start
	  if (prevstart) start = 1;
	prevstart = PORTAD0_PTAD0;
	
	PTT_PTT0 = 1; // send pulse
	PTT_PTT0 = 0;
	
	if (!PORTAD0_PTAD0)	// get up
	  if (prevup) up = 1;
	prevup = PORTAD0_PTAD0;
	
	PTT_PTT0 = 1; // send pulse
	PTT_PTT0 = 0;
	
	if (!PORTAD0_PTAD0)	// get down
	  if (prevdown) down = 1;
	prevdown = PORTAD0_PTAD0;
	
	PTT_PTT0 = 1; // send pulse
	PTT_PTT0 = 0;
	
	if (!PORTAD0_PTAD0)	// get left
	  if (prevleft) left = 1;
	prevleft = PORTAD0_PTAD0;
	
	PTT_PTT0 = 1; // send pulse
	PTT_PTT0 = 0;
	
	if (!PORTAD0_PTAD0)	// get right
	  if (prevright) right = 1;
	prevright = PORTAD0_PTAD0;
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
