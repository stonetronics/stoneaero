#include "Button.h"

Button::Button( int pin )
{
  setPin(pin);
  this->pin = pin;
}

Button::Button(int pin, void(*onPress)(void), void (*onRelease)( void ))
{
  setPin(pin);
  registerOnPress(onPress);
  registerOnRelease(onRelease);
}

void Button::setPin(int pin)
{
  this->pin = pin;
  pinMode(pin, INPUT);
  btn = digitalRead(pin);
  btnOld = btn;
}

void Button::registerOnPress(void (*onPress)( void ))
{
  this->onPress = onPress;
}

void Button::registerOnRelease(void (*onRelease)( void ))
{
  this->onRelease = onRelease;
}

void Button::update(void)
{
  btn = digitalRead(pin);

  if (btn != btnOld)
  {
    btnOld = btn;
    if (btn) //rising edge = button release
    {
      if (onRelease != NULL)
      {
        onRelease();
      }
    } else { //falling edge = button press
      if (onPress != NULL)
      {
        onPress();
      }
    }
  }
}

ButtonEdge Button::updateEdge( void )
{
  btn = digitalRead(pin);

  if (btn != btnOld)
  {
    btnOld = btn;
    if (btn) //rising edge = button release
    {
      return BE_RISING;
    } else { //falling edge = button press
      return BE_FALLING;
    }
  }

  //in case nothing happened:
  return BE_NONE;
}
