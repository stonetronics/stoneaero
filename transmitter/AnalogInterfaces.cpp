#include "AnalogInterfaces.h"
#include <EEPROM.h>

#define JOY_R_X     A0
#define JOY_R_X_MIN_ADD   ANALOGEXTREMES_BASEADDRESS
#define JOY_R_X_MAX_ADD   ANALOGEXTREMES_BASEADDRESS + 2
#define JOY_R_Y     A1
#define JOY_R_Y_MIN_ADD   ANALOGEXTREMES_BASEADDRESS + 4
#define JOY_R_Y_MAX_ADD   ANALOGEXTREMES_BASEADDRESS + 6
#define JOY_L_X     A2
#define JOY_L_X_MIN_ADD   ANALOGEXTREMES_BASEADDRESS + 8
#define JOY_L_X_MAX_ADD   ANALOGEXTREMES_BASEADDRESS + 10
#define JOY_L_Y     A3
#define JOY_L_Y_MIN_ADD   ANALOGEXTREMES_BASEADDRESS + 12
#define JOY_L_Y_MAX_ADD   ANALOGEXTREMES_BASEADDRESS + 14

#define POTI_A  A6
#define POTI_A_MIN_ADD   ANALOGEXTREMES_BASEADDRESS + 16
#define POTI_A_MAX_ADD   ANALOGEXTREMES_BASEADDRESS + 18
#define POTI_B  A7
#define POTI_B_MIN_ADD   ANALOGEXTREMES_BASEADDRESS + 20
#define POTI_B_MAX_ADD   ANALOGEXTREMES_BASEADDRESS + 22

AnalogInterfaces::AnalogInterfaces()
{
    joyRXmin = 0;
    joyRXmax = 1023;
    joyRYmin = 1023;
    joyRYmax = 0;
    joyLXmin = 0;
    joyLXmax = 1023;
    joyLYmin = 1023;
    joyLYmax = 0;
    
    potiAmin = 0;
    potiAmax = 1023;
    potiBmin = 0;
    potiBmax = 1023;
}

void AnalogInterfaces::write16BitEEPROM(int16_t val, uint16_t addr)
{
  EEPROM.write((val&0xFF), addr++);
  EEPROM.write(((val&0xFF)>>8), addr);
}

int16_t AnalogInterfaces::read16BitEEPROM(uint16_t addr)
{
  int16_t retVal;

  retVal = EEPROM.read(addr++);
  retVal += EEPROM.read(addr)<<8;

  return retVal;
}

void AnalogInterfaces::storeExtremes( void )
{
  write16BitEEPROM(joyRXmin, JOY_R_X_MIN_ADD);
  write16BitEEPROM(joyRXmax, JOY_R_X_MAX_ADD);
  write16BitEEPROM(joyRYmin, JOY_R_Y_MIN_ADD);
  write16BitEEPROM(joyRYmax, JOY_R_Y_MAX_ADD);
  write16BitEEPROM(joyLXmin, JOY_L_X_MIN_ADD);
  write16BitEEPROM(joyLXmax, JOY_L_X_MAX_ADD);
  write16BitEEPROM(joyLYmin, JOY_L_Y_MIN_ADD);
  write16BitEEPROM(joyLYmax, JOY_L_Y_MAX_ADD);

  write16BitEEPROM(potiAmin, POTI_A_MIN_ADD);
  write16BitEEPROM(potiAmax, POTI_A_MAX_ADD);
  write16BitEEPROM(potiBmin, POTI_B_MIN_ADD);
  write16BitEEPROM(potiBmax, POTI_B_MAX_ADD);
}

void AnalogInterfaces::retrieveExtremes( void )
{
  joyRXmin = read16BitEEPROM(JOY_R_X_MIN_ADD);
  joyRXmax = read16BitEEPROM(JOY_R_X_MAX_ADD);
  joyRYmin = read16BitEEPROM(JOY_R_Y_MIN_ADD);
  joyRYmax = read16BitEEPROM(JOY_R_Y_MAX_ADD);
  joyLXmin = read16BitEEPROM(JOY_L_X_MIN_ADD);
  joyLXmax = read16BitEEPROM(JOY_L_X_MAX_ADD);
  joyLYmin = read16BitEEPROM(JOY_L_Y_MIN_ADD);
  joyLYmax = read16BitEEPROM(JOY_L_Y_MAX_ADD);

  potiAmin = read16BitEEPROM(POTI_A_MIN_ADD);
  potiAmax = read16BitEEPROM(POTI_A_MAX_ADD);
  potiBmin = read16BitEEPROM(POTI_B_MIN_ADD);
  potiBmax = read16BitEEPROM(POTI_B_MAX_ADD);
}

int8_t AnalogInterfaces::getRX( void )
{
  return (int8_t) map(analogRead(JOY_R_X), joyRXmin, joyRXmax, -90, 90);
}

int8_t AnalogInterfaces::getRY( void )
{
  return (int8_t) map(analogRead(JOY_R_Y), joyRYmin, joyRYmax, -90, 90);
}

int8_t AnalogInterfaces::getLX( void )
{
  return (int8_t) map(analogRead(JOY_L_X), joyLXmin, joyLXmax, -90, 90);
}

int8_t AnalogInterfaces::getLY( void )
{
  return (int8_t) map(analogRead(JOY_L_Y), joyLYmin, joyLYmax, -90, 90);
}

uint8_t AnalogInterfaces::getPotiA( void )
{
  return (uint8_t) map(analogRead(POTI_A), potiAmin, potiAmax, 0, 100);
}

int8_t AnalogInterfaces::getPotiAServoScale( void )
{
  return (int8_t) map(analogRead(POTI_A), potiAmin, potiAmax, -90, 90);
}

uint8_t AnalogInterfaces::getPotiB( void )
{
  return (uint8_t) map(analogRead(POTI_B), potiBmin, potiBmax, 0, 100);
}

