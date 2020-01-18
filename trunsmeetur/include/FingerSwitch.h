#ifndef __FINGERSWITCH_H__
#define __FINGERSWITCH_H__

#include<Arduino.h>

class FingerSwitch
{
  public:
    FingerSwitch(int pin, uint8_t invert); //invert inverts the signal on pin which gives 1 or 0 depending on the switch
    FingerSwitch(int pin, uint8_t invert, uint8_t* state);
    uint8_t get( void ); //read switch
    void registerState( uint8_t* state ); //register a place in memory where the state of the switch is stored
    void update( void );
  private:
    int pin;
    uint8_t invert;
    uint8_t* state = NULL;
};

#endif
