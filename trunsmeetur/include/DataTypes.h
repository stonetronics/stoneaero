#ifndef __GLOBALDATA_H__
#define __GLOBALDATA_H__

#include <Arduino.h>
#include "Joystick.h"
#include "ControlInterface.h"
#include "Messages.h"

typedef struct
{
  int8_t roll;
  int8_t pitch;
  int8_t yaw;
  int8_t thrust;
} ControlVector;

typedef struct
{
  ControlInterfaceConfiguration controlInterfaceConfiguration;
} PersistentData;

#define PERSISTENTDATA_EEPROM_ADDR   (0x0A)

typedef struct
{
  PersistentData persistent;
  Report lastReport;
} GlobalData;

#endif
