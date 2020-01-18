#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include "Button.h"

typedef struct
{
    int16_t min;
    int16_t max;
    int8_t offset;
    int16_t normMin;
    int16_t normMax;
} JoystickConfiguration;

class Joystick
{
  public:
    Joystick(int pin, Button* incBtn, Button* decBtn, JoystickConfiguration* configuration);
    Joystick(int pin, Button* incBtn, Button* decBtn, JoystickConfiguration* configuration, int16_t* state);
    void registerState(int16_t* state) {this->state = state; };
    void update( void ); //update value and buttons
    void learnMax( void ); //take current position as maximum
    void learnMin( void ); //take current position as minimum
    int16_t get( void ); //get Joystick position, mapped to the range as set in the configuration

  private:
    int pin;
    Button* incBtn;
    Button* decBtn;
    JoystickConfiguration* configuration;
    int16_t* state;
};



#endif
