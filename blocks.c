#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

char iblock1[3][1] = {
  {1},
  {1},
  {1}
};

char iblock2[1][3] = {1,1,1};

char tblock1[2][3] = {
  {0,2,0},
  {2,2,2}
};

char tblock2[3][2] = {
  {2,0},
  {2,2},
  {2,0}
};

char tblock3[2][3] = {
  {2,2,2},
  {0,2,0}
};

char tblock4[3][2] = {
  {0,2},
  {2,2},
  {0,2}
};

char zblock1[2][3] = {
  {3,3,0},
  {0,3,3},
};

char zblock2[3][2] = {
  {0,3},
  {3,3},
  {3,0}
};

char sblock1[2][3] = {
  {0,4,4},
  {4,4,0},
};

char sblock2[3][2] = {
  {4,0},
  {4,4},
  {0,4}
};

char jblock1[2][3] = {
  {5,0,0},
  {5,5,5}
};

char jblock2[3][2] = {
  {5,5},
  {5,0},
  {5,0}
};

char jblock3[2][3] = {
  {5,5,5},
  {0,0,5}
};

char jblock4[3][2] = {
  {0,5},
  {0,5},
  {5,5}
};

char lblock1[2][3] = {
  {0,0,6},
  {6,6,6}
};

char lblock2[3][2] = {
  {6,0},
  {6,0},
  {6,6}
};

char lblock3[2][3] = {
  {6,6,6},
  {6,0,0}
};

char lblock4[3][2] = {
  {6,6},
  {0,6},
  {0,6}
};


void main(void) {
  /* put your own code here */
  


	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
