#include "ControlInterface.h"

ControlInterface::ControlInterface( Joystick* rollJoy, Joystick* pitchJoy, Joystick* yawJoy, Joystick* throttleJoy, IdleSwitch* idleSwitch, FingerSwitch* elevDRswitch, FingerSwitch* ailerDRswitch, FingerSwitch* thHoldSwitch, FingerSwitch* gyroSensSwitch, Poti* pitAdjPoti )
{
  this->rollJoy=rollJoy;
  this->pitchJoy=pitchJoy;
  this->yawJoy=yawJoy;
  this->throttleJoy=throttleJoy;
  this->idleSwitch=idleSwitch;
  this->elevDRswitch=elevDRswitch;
  this->ailerDRswitch=ailerDRswitch;
  this->thHoldSwitch=thHoldSwitch;
  this->gyroSensSwitch=gyroSensSwitch;
  this->pitAdjPoti=pitAdjPoti;

  update(); //update everything in the first place to get samples
}

void ControlInterface::update( void )
{
  rollJoy->update();
  pitchJoy->update();
  yawJoy->update();
  throttleJoy->update();
  idleSwitch->update();
  elevDRswitch->update();
  ailerDRswitch->update();
  thHoldSwitch->update();
  gyroSensSwitch->update();
  pitAdjPoti->update();
}
