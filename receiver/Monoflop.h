#include <Arduino.h> //for integer definition


class Monoflop
{
  public:
    Monoflop( void );
    void trigger( void ){lastTriggerTime = millis();};
    uint8_t output( void );
    void setHoldTime( int holdTime ){this->holdTime = holdTime;};
    
  private:
    int lastTriggerTime;
    int holdTime;
};

