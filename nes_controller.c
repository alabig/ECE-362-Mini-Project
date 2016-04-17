#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <stdlib.h>

#define TETRIS 4

#define NUMROWS 23  // 3 extra rows to allow movement as piece appears
#define NUMCOLS 10

#define NUMPIECES 7

char x, y, size, rot;
char row, col;
char emptyrow;
char *** block;
char ** currentpiece;

char tick, timcnt, level;
char roundover, gameover;

char up, down, left, right, a, b;

char n, i;

char rclear[4] = {0,0,0,0};  // rows to clear

char gameboard[NUMROWS][NUMCOLS];

int score, highscore;

char iblock[4][4][4] = {
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
};

char tblock[4][3][3] = {
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
};

char zblock[4][3][3] = {
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
};

char sblock[4][3][3] = {
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
};

char jblock[4][3][3] = {
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
};

char lblock[4][3][3] = {
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
};

char oblock[4][3][3] = {
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
};

void initializations(void)
{
  
  /* Initialize TIM Ch 7 (TC7) for periodic interrupts every 100 ms */
  TSCR1 = 0x80; // enable timer subsystem
  TIOS  = 0x80; // set channel 7 for output compare
  TSCR2 = 0x0C; // set appropriate pre-scale factor and enable counter reset after OC7
  TC7   = 15000;// set up channel 7 to generate 100 ms interrupt rate
  TIE   = 0x80; // enable TIM Ch 7 interrupts
  
  //initialize gameboard
  for (y = 0;y < NUMROWS;y++)
    for (x = 0;x < NUMCOLS;x++) gameboard[y][x] = 0;
    
  // other initializations
  roundover = 1;
  gameover = 0;
  level = 0;
  tick = 0;
  timcnt = 0;
  
}

int checkcollision(void)
{
  int collision = 0;
  
  for (y = 0; y < size; y++)
  {
    for (x = 0; x < size; x++)
    {
      if (currentpiece[size - 1 - y][x])
        if ((col + x < 0) || (col + x > 9) || (row + y < 0) || (gameboard[row + y][col + x])) collision = 1;
    }
  }
    
  return collision;
}

void clearpiece(void)
{
  for (y = 0; y < size; y++)
    for (x = 0; x < size; x++)
      if (currentpiece[size - 1 - y][x]) gameboard[row + y][col + x] = 0;
}

void setpiece(void)
{
  for (y = 0; y < size; y++)
    for (x = 0; x < size; x++)
      if (currentpiece[size - 1 - y][x]) gameboard[row + y][col + x] = 1;
}

void flashrows(void)
{
  // flashing effect for clear rows
}

void clearrows(void)
{
  i = 0;
  row = rclear[i];
  while (!emptyrow && (i < 4))
  {
    emptyrow = 1;
    while (row == rclear[i])
    {
      row++;
      i++;
    }
    for (x = 0; x < size; x++)
    {
      gameboard[row-(i+1)][col + x] = gameboard[row][col + x];
      if (gameboard[row][col + x]) emptyrow = 0;
    }
    row++;
  }
}

void updatedisp(void)
{
  // probably will become an interrupt - update game board and display
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
        
        n = (char)(rand() % (NUMPIECES-1)); // set new piece as currentpiece
        switch (n)
        {
          case 0:  block = iblock;
          break;
          case 1:  block = tblock;
          break;
          case 2:  block = zblock;
          break;
          case 3:  block = sblock;
          break;
          case 4:  block = jblock;
          break;
          case 5:  block = lblock;
          break;
          case 6:  block = oblock;
        }
        
        // reset row and col for new piece
        row = NUMROWS - 4;
        col = NUMCOLS - 7;
        
        if (checkcollision()) gameover = 1;  // unable to initially place new piece, game over
        else updatedisp();  // update board with initial placement of new piece
      }
      else
      {
        row--;
        if (checkcollision())
        {
          row++;
          roundover = 1;
        }
        else
        {
          row++;
          clearpiece();
          row--;
          setpiece();
          updatedisp();
        }
      }
    }
    
    if (left)
    {
      col--;
      if (checkcollision()) col++;
      else
      {
        col++;
        clearpiece();
        col--;
        setpiece();
      }
    }
    else if (right)
    {
      col++;
      if (checkcollision()) col--;
      else
      {
        col--;
        clearpiece();
        col++;
        setpiece();
      }
    }
    else if (a) // clockwise
    {
      rot = (char)(++rot % 4);  //update current piece
      currentpiece = block[rot];
      if (checkcollision()) //undo update
      {
        rot = (char)(--rot % 4);
        currentpiece = block[rot];
      }
      else
      {
        rot = (char)(--rot % 4);
        currentpiece = block[rot];
        clearpiece();
        rot = (char)(++rot % 4);
        currentpiece = block[rot];
        setpiece();
      }
    }
    else if (b) // counterclockwise
    {
      clearpiece();
      rot = (char)(--rot % 4);  //update current piece
      currentpiece = block[rot];
      if (checkcollision()) //undo update
      {
        rot = (char)(++rot % 4);
        currentpiece = block[rot];
      }
      setpiece();
    }
    else if (down)
    {
      row--;
      if (checkcollision())
      {
        row++;
        roundover = 1;
      }
      else
      {
        row++;
        clearpiece();
        row--;
        setpiece();
      }
    }
    else if (up)  // drop piece
    {
      clearpiece();
      do row--;
      while (!checkcollision());
      row++;
      roundover = 1;
      setpiece();
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
 	
 	timcnt++;
 	if (timcnt == (10 - level))
 	{
 	  timcnt = 0;
 	  tick = 1;
 	}
 	
}
