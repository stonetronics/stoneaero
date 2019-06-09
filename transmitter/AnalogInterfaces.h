#include <Arduino.h> //for integer definitions

#define ANALOGEXTREMES_BASEADDRESS  10

class AnalogInterfaces
{
  public:
  
    AnalogInterfaces(); //constructor
  
    void setRXextremes(int16_t joyRXmin, int16_t joyRXmax){this->joyRXmin = joyRXmin; this->joyRXmax = joyRXmax;};
    void setRYextremes(int16_t joyRYmin, int16_t joyRYmax){this->joyRYmin = joyRYmin; this->joyRYmax = joyRYmax;};
    void setLXextremes(int16_t joyLXmin, int16_t joyLXmax){this->joyLXmin = joyLXmin; this->joyLXmax = joyLXmax;};
    void setLYextremes(int16_t joyLYmin, int16_t joyLYmax){this->joyLYmin = joyLYmin; this->joyLYmax = joyLYmax;};
    
    void setPotiAextremes(int16_t potiAmin, int16_t potiAmax){this->potiAmin = potiAmin; this->potiAmax = potiAmax;};
    void setPotiBextremes(int16_t potiBmin, int16_t potiBmax){this->potiBmin = potiBmin; this->potiBmax = potiBmax;};

    void storeExtremes( void ); //stores the currently set extremes in eeprom
    void retrieveExtremes( void ); //overwrite the currently set extremes with the values stored in eeprom

    int8_t getRX( void ); //measures the joystick potentiometer and outputs the value mapped to [-90, 90]
    int8_t getRY( void ); //measures the joystick potentiometer and outputs the value mapped to [-90, 90]
    int8_t getLX( void ); //measures the joystick potentiometer and outputs the value mapped to [-90, 90]
    int8_t getLY( void ); //measures the joystick potentiometer and outputs the value mapped to [-90, 90]

    uint8_t getPotiA( void ); //measures the potentiometer and outputs the value mapped to [0,100]
    int8_t getPotiAServoScale( void );//measures the potentiometer and outputs the value mapped to [-90, 90]
    uint8_t getPotiB( void ); //measures the potentiometer and outputs the value mapped to [0,100]

  private:
    void write16BitEEPROM(int16_t val, uint16_t addr);
    int16_t read16BitEEPROM(uint16_t addr);
  
    int16_t joyRXmin;
    int16_t joyRXmax;
    int16_t joyRYmin;
    int16_t joyRYmax;
    int16_t joyLXmin;
    int16_t joyLXmax;
    int16_t joyLYmin;
    int16_t joyLYmax;
    
    int16_t potiAmin;
    int16_t potiAmax;
    int16_t potiBmin;
    int16_t potiBmax;
};

