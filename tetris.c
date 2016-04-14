#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#define NUMROWS 23  // 3 extra rows to allow movement as piece appears
#define NUMCOLS 10

char x, y, size;
char row, col;
char ** currentpiece;

char gameboard[NUMROWS][NUMCOLS];

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

void initboard(void)
{
  for (y = 0;y < NUMROWS;y++)
    for (x = 0;x < NUMCOLS;x++) gameboard[y][x] = 0;
  
}

int checkcollision(void)
{
  int collision = 0;
  
  for (y = 0; y < size; y++)
  {
    for (x = 0; x < size; x++)
    {
      if (currentpiece[y][x])
        if ((col + x < 0) || (col + x > 9) || (row + y < 0) || (gameboard[row + y][col + x])) collision = 1;
    }
  }
    
  return collision;
}

void main(void) {
  /* put your own code here */
  
  	EnableInterrupts;

  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
