#include "TransmitterUtils.h"

int8_t limitInt8(int16_t toLimit)
{
  if (toLimit > 127)
  {
    return 127;
  } else if (toLimit < -128)
  {
    return -128;
  } else {
    return (int8_t) toLimit;
  }
}
