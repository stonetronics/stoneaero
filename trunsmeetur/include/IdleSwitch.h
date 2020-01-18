#ifndef __IDLESWITCH_H__
#define __IDLESWITCH_H__

#include<Arduino.h>

typedef enum
{
  IS_N = 0, IS_0 = 1, IS_1 = 2
} IdleSwitchState;

class IdleSwitch
{
  public:
    IdleSwitch(int pin1, int pinN);
    IdleSwitch(int pin1, int pinN, IdleSwitchState* state);
    IdleSwitchState get( void );
    void registerState( IdleSwitchState* state);
    void update( void );
  private:
    int pin1;
    int pinN;
    IdleSwitchState* state = NULL;
};

#endif
