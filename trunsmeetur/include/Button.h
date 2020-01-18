#ifndef __BUTTON_H__
#define __BUTTON_H__


#include <Arduino.h>

typedef enum {
  BE_RISING, BE_FALLING, BE_NONE
} ButtonEdge;

class Button
{
  public:
    Button( int pin ); //pin to assign the button to
    Button(int pin, void(*onPress)(void), void (*onRelease)( void )); //pin is the pin the button is connected to, onPress is a function to be called when the button is pressed (0-active), onRelease is a function that gets called when the button is released (0-active)
    void setPin(int pin); //pin is the pin the button is connected to
    void registerOnPress(void (*onPress)( void )); // onPress is a function that gets called when the button is pressed (0-active)
    void registerOnRelease(void (*onRelease)( void )); // onRelease is a function that gets called when the button is released (0-active)
    void update( void );
    ButtonEdge updateEdge( void );
  private:
    uint8_t pin;
    uint8_t btn;
    uint8_t btnOld;
    void (*onPress)( void );
    void (*onRelease)( void );
};

#endif
