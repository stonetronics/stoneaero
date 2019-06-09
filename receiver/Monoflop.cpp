#include "Monoflop.h"

Monoflop::Monoflop( void )
{
  lastTriggerTime = 0;
}

uint8_t Monoflop::output( void )
{
  if (millis() > (lastTriggerTime + holdTime))
    return 0;
  else
    return 1;
}

