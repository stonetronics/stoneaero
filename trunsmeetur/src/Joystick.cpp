#include "Joystick.h"

#define OFFSET_INCDECSTEP 1

Joystick::Joystick(int pin, Button* incBtn, Button* decBtn, JoystickConfiguration* configuration)
{
  this->pin = pin;
  this->incBtn = incBtn;
  this->decBtn = decBtn;
  this->configuration = configuration;

  incBtn->updateEdge(); //sample the buttons first time
  decBtn->updateEdge();

  pinMode(pin, INPUT);
}

Joystick::Joystick(int pin, Button* incBtn, Button* decBtn, JoystickConfiguration* configuration, int16_t* state)
{
  this->pin = pin;
  this->incBtn = incBtn;
  this->decBtn = decBtn;
  this->configuration = configuration;
  this->state = state;

  incBtn->updateEdge(); //sample the buttons first time
  decBtn->updateEdge();

  pinMode(pin, INPUT);
}

void Joystick::update( void )
{
  if(incBtn->updateEdge()==BE_FALLING) //offset increment button pressed
  {
    configuration->offset += OFFSET_INCDECSTEP;
  }

  if(decBtn->updateEdge()==BE_FALLING) //offset decrement button pressed
  {
    configuration->offset -= OFFSET_INCDECSTEP;
  }

  *state = get();
}

void Joystick::learnMax( void )
{
  configuration->max = analogRead(pin);
}

void Joystick::learnMin( void )
{
  configuration->min = analogRead(pin);
}

int16_t Joystick::get( void )
{
  return map( analogRead(pin), //read joystick poti pin and map it from
                configuration->min, configuration->max, //input range to
                configuration->normMin, configuration->normMax) //output range
                + configuration->offset; //add the offset to shift neutral to 0
}
