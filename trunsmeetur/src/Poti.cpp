#include"Poti.h"

Poti::Poti(int pin, PotiConfiguration* configuration)
{
  this->pin = pin;
  this->configuration = configuration;
}

Poti::Poti(int pin, PotiConfiguration* configuration, int16_t* state)
{
  this->state = state;
  this->pin = pin;
  this->configuration = configuration;
}

int16_t Poti::get( void )
{
  return map(analogRead(pin),
              configuration->min,configuration->max,
              configuration->outMin,configuration->outMax);
}

void Poti::update( void )
{
  if (state != NULL)
  {
    *state = get();
  }
}

void Poti::learnMax( void )
{
  configuration->max = analogRead(pin);
}

void Poti::learnMin( void )
{
  configuration->min = analogRead(pin);
}
