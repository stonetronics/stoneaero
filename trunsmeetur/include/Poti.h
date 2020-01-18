#ifndef __POTI_H__
#define __POTI_H__

#include<Arduino.h>

typedef struct {
  uint16_t min;
  uint16_t max;
  int16_t outMin;
  int16_t outMax;
} PotiConfiguration;

class Poti
{
  public:
    Poti(int pin, PotiConfiguration* configuration);
    Poti(int pin, PotiConfiguration* configuration, int16_t* state);
    void registerState( int16_t* state ) {this->state = state;};
    int16_t get( void );
    void update( void );
    void learnMax( void );
    void learnMin( void );
  private:
    int pin;
    PotiConfiguration* configuration;
    int16_t* state = NULL;
};

#endif
