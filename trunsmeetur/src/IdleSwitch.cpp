#include "IdleSwitch.h"

IdleSwitch::IdleSwitch(int pin1, int pinN)
{
  this->pin1 = pin1;
  this->pinN = pinN;
  pinMode(pin1, INPUT);
  pinMode(pinN, INPUT);
}

IdleSwitch::IdleSwitch(int pin1, int pinN, IdleSwitchState* state)
{
  this->state = state;
  this->pin1 = pin1;
  this->pinN = pinN;
  pinMode(pin1, INPUT);
  pinMode(pinN, INPUT);
}

IdleSwitchState IdleSwitch::get( void )
{
  if (digitalRead(pinN) == LOW) //switch in N position
  {
    return IS_N;
  } else if (digitalRead(pin1) == LOW) //switch in 1 position
  {
    return IS_1;
  }
  //the switch isn't connected in any other case
  return IS_0;
}

void IdleSwitch::registerState( IdleSwitchState* state)
{
  this->state = state;
}

void IdleSwitch::update( void )
{
  if (state != NULL)
  {
    *state = get();
  }
}
