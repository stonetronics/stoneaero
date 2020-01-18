#ifndef __SERVODEF_H__
#define __SERVODEF_H__

#include "pindef.h"

//define wich servo is attached to which channel and also define maximum servo angles

#define MOTOR_PIN     CHAN3
#define MOTOR_MAX     180
#define MOTOR_MIN     10

#define RUDDER_PIN    CHAN6
#define RUDDER_MAX    170
#define RUDDER_MIN    10

#define ELEVATOR_PIN  CHAN7
#define ELEVATOR_MAX  160
#define ELEVATOR_MIN  20

#define AILERONRIGHT_PIN   CHAN4
#define AILERONRIGHT_MAX   25
#define AILERONRIGHT_MIN   155

#define AILERONLEFT_PIN   CHAN5
#define AILERONLEFT_MAX   155
#define AILERONLEFT_MIN   25

#endif
