#include "FingerSwitch.h"

FingerSwitch::FingerSwitch(int pin, uint8_t invert)
{
  this->invert = invert;
  this->pin = pin;
  pinMode(pin, INPUT);
}

FingerSwitch::FingerSwitch(int pin, uint8_t invert, uint8_t* state)
{
  this->state = state;
  this->invert = invert;
  this->pin = pin;
  pinMode(pin, INPUT);

}

uint8_t FingerSwitch::get( void )
{
  if (!invert)
  {
    return digitalRead(pin);
  } else {
    return !digitalRead(pin);
  }
}


void FingerSwitch::registerState( uint8_t* state )
{
  this->state = state;
}

void FingerSwitch::update( void )
{
  if (state != NULL)
  {
    *state = get();
  }
}
