#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <Arduino.h>

#define COMMANDPIPE   1
#define REPORTPIPE    2

typedef struct {
  uint8_t mode; //'c' = command; 'o' = offsets
  int8_t throttle;
  int8_t yaw;
  int8_t pitch;
  int8_t roll;
  uint8_t controlByte; //0x01 = airbrakes
} Command;

typedef struct {
  int16_t current;
  int16_t cellVoltage[3];
} Report;

#endif
