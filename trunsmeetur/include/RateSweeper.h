#include <Arduino.h>

class RateSweeper
{
  public:

    RateSweeper( void ){rate = 1;};

    void nextRate( void );

    void setRate(uint8_t rate){this->rate = rate;};

    uint8_t getRate( void ){return rate;};
    
  private:

    uint8_t rate;
};

