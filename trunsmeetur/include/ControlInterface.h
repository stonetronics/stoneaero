#ifndef __CONTROLINTERFACE_H__
#define __CONTROLINTERFACE_H__

#include "Joystick.h"
#include "IdleSwitch.h"
#include "FingerSwitch.h"
#include "Poti.h"

typedef struct
{
  JoystickConfiguration rollJoyConf;
  JoystickConfiguration pitchJoyConf;
  JoystickConfiguration yawJoyConf;
  JoystickConfiguration throttleJoyConf;
  PotiConfiguration pitAdjConf;
} ControlInterfaceConfiguration;

class ControlInterface
{
  public:
    ControlInterface(   Joystick* rollJoy,
                        Joystick* pitchJoy,
                        Joystick* yawJoy,
                        Joystick* throttleJoy,
                        IdleSwitch* idleSwitch,
                        FingerSwitch* elevDRswitch,
                        FingerSwitch* ailerDRswitch,
                        FingerSwitch* thHoldSwitch,
                        FingerSwitch* gyroSensSwitch,
                        Poti* pitAdjPoti);

    void update( void );

    void learnRollJoyMax( void ) { rollJoy->learnMax(); };
    void learnRollJoyMin( void ) { rollJoy->learnMin(); };
    void learnPitchJoyMax( void ) { pitchJoy->learnMax(); };
    void learnPitchJoyMin( void ) { pitchJoy->learnMin(); };
    void learnYawJoyMax( void ) { yawJoy->learnMax(); };
    void learnYawJoyMin( void ) { yawJoy->learnMin(); };
    void learnThrottleJoyMax( void ) { throttleJoy->learnMax(); };
    void learnThrottleJoyMin( void ) { throttleJoy->learnMin(); };
    void learnPitAdjMax( void ) { pitAdjPoti->learnMax(); };
    void learnPitAdjMin( void ) { pitAdjPoti->learnMin(); };

  private:
    Joystick* rollJoy;
    Joystick* pitchJoy;
    Joystick* yawJoy;
    Joystick* throttleJoy;
    IdleSwitch* idleSwitch;
    FingerSwitch* elevDRswitch;
    FingerSwitch* ailerDRswitch;
    FingerSwitch* thHoldSwitch;
    FingerSwitch* gyroSensSwitch;
    Poti* pitAdjPoti;
};

#endif
