#include "RateSweeper.h"

void RateSweeper::nextRate( void )
{
  rate++;
  if (rate == 7)
    rate = 1;
}

