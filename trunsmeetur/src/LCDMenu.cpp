#include "LCDMenu.h"
#include "pindef.h"
#include <EEPROM.h>

LCDMenu::LCDMenu( LiquidCrystal* lcd , Button* okBtn, Button* rightBtn, Button* leftBtn, Button* downBtn, Button* upBtn, ControlInterface* controlInterface, GlobalData* globalData )
{
  this->lcd = lcd;
  this->okBtn = okBtn;
  this->rightBtn = rightBtn;
  this->leftBtn = leftBtn;
  this->downBtn = downBtn;
  this->upBtn = upBtn;
  this->controlInterface = controlInterface;
  this->globalData = globalData;

  millisPrev = millis();

  //init frame and button
  currentFrame = FR_START;

  lcd->begin(16, 2); //set #columns,#lines
  lcd->print("hullo trunsmeet!");
  lcd->setCursor(0, 1); //set column(0-15),line(0-1)
  lcd->print(" =stonetronics= ");
}

void LCDMenu::update( void )
{
  float voltages[3]; //helper for displaying
  float power;
  int millisNow;

  //do all the processing of the frames in a state machine like structure
  switch( currentFrame )
  {
    case FR_START:
      if (okBtn->updateEdge() == BE_FALLING)
      {
        setHome();
      }
      break;

    case FR_HOME:
      if(upBtn->updateEdge() == BE_FALLING)
      {
        setJoystickSettings();
      }

      if(downBtn->updateEdge() == BE_FALLING)
      {
        setEnergyData();
      }
      break;

    case FR_ENERGY_DATA:
      //menu for displaying all energy data

      if(upBtn->updateEdge() == BE_FALLING)
      {
        setHome();
      }

      if(downBtn->updateEdge() == BE_FALLING)
      {
        setJoystickSettings();
      }

      if(rightBtn->updateEdge() == BE_FALLING)
      {
        setCells();
      }

      if(leftBtn->updateEdge() == BE_FALLING)
      {
        setCharge();
      }
      break;

    case FR_CELLS:
      //update values on display from global data buffer

      //convert them from mV to V beforehand
      for (uint8_t k = 0; k < 3; k++)
      {
        voltages[k] = (float)(globalData->lastReport.cellVoltage[k]) / 1000.0;
      }

      displayNumber(0, 1, voltages[0], 1, 2);
      displayNumber(6, 1, voltages[1], 1, 2);
      displayNumber(12, 1, voltages[2], 1, 2);

      //provide link to other frames
      if(leftBtn->updateEdge() == BE_FALLING)
      {
        setEnergyData();
      }

      if(rightBtn->updateEdge() == BE_FALLING)
      {
        setCurrent();
      }
      break;

    case FR_CURRENT:
      //update values on display from global data buffer

      displayNumberWithSign(1, 1, (globalData->lastReport.current), 5);

      //provide link to other frames
      if(leftBtn->updateEdge() == BE_FALLING)
      {
        setCells();
      }

      if(rightBtn->updateEdge() == BE_FALLING)
      {
        setCellsCurrent();
      }
      break;

    case FR_CELLS_CURRENT:
      //update values on display from global data buffer

      //convert voltages from mV to V beforehand
      for (uint8_t k = 0; k < 3; k++)
      {
        voltages[k] = (float)(globalData->lastReport.cellVoltage[k]) / 1000.0;
      }

      //first line voltages
      displayNumber(0, 0, voltages[0], 1, 2);
      displayNumber(6, 0, voltages[1], 1, 2);
      displayNumber(12, 0, voltages[2], 1, 2);

      //second line current
      displayNumberWithSign(1, 1, (globalData->lastReport.current), 5);

      //provide link to other frames
      if(leftBtn->updateEdge() == BE_FALLING)
      {
        setCurrent();
      }

      if(rightBtn->updateEdge() == BE_FALLING)
      {
        setPower();
      }
      break;

    case FR_POWER:
      //update values on display from global data buffer
      power = ((float)(globalData->lastReport.current) / 1000.0) * ((float)(globalData->lastReport.cellVoltage[0]+globalData->lastReport.cellVoltage[2]+globalData->lastReport.cellVoltage[2]) / 1000.0);
      displayNumber(2, 1, power, 3, 3);

      //provide link to other frames
      if(leftBtn->updateEdge() == BE_FALLING)
      {
        setCellsCurrent();
      }

      if(rightBtn->updateEdge() == BE_FALLING)
      {
        setEnergy();
      }
      break;

    case FR_ENERGY:
      //update values on display from global data buffer
      millisNow = millis();
      power = ((float)(globalData->lastReport.current) / 1000.0) * ((float)(globalData->lastReport.cellVoltage[0]+globalData->lastReport.cellVoltage[2]+globalData->lastReport.cellVoltage[2]) / 1000.0);
      integral += power * ((float)(millisNow - millisPrev) / 1000.0) / 3600.0; //conversion to watthours
      millisPrev = millisNow;
      displayNumberWithSign(1, 1, integral, 3, 3);

      //reset on ok
      if(okBtn->updateEdge() == BE_FALLING)
      {
        integral = 0;
      }

      //provide link to other frames
      if(leftBtn->updateEdge() == BE_FALLING)
      {
        setPower();
      }

      if(rightBtn->updateEdge() == BE_FALLING)
      {
        setCharge();
      }
      break;

    case FR_CHARGE:
      //update values on display from global data buffer
      millisNow = millis();
      integral += ((float)(millisNow - millisPrev) / 1000.0) * (float)(globalData->lastReport.current) / 1000.0 / 3600.0; //conversion to amphours
      millisPrev = millisNow;
      displayNumberWithSign(1, 1, integral, 3, 3);

      //reset on ok
      if(okBtn->updateEdge() == BE_FALLING)
      {
        integral = 0;
      }

      //provide link to other frames
      if(leftBtn->updateEdge() == BE_FALLING)
      {
        setEnergy();
      }

      if(rightBtn->updateEdge() == BE_FALLING)
      {
        setEnergyData();
      }
      break;

    case FR_JOSTICK_SETTINGS:
      //menu for all settings joystick related

      if(upBtn->updateEdge() == BE_FALLING)
      {
        setEnergyData();
      }

      if(downBtn->updateEdge() == BE_FALLING)
      {
        setHome();
      }

      if(rightBtn->updateEdge() == BE_FALLING)
      {
        setSendOffsets();
      }

      if(leftBtn->updateEdge() == BE_FALLING)
      {
        setRestoreCalibration();
      }
      break;

    case FR_SEND_OFFSETS:
      //on ok press, do the action
      if(okBtn->updateEdge() == BE_FALLING)
      {
        //do the action here
      }

      //provide link to other frames
      if(leftBtn->updateEdge() == BE_FALLING)
      {
        setJoystickSettings();
      }

      if(rightBtn->updateEdge() == BE_FALLING)
      {
        setCalibrateJoysticks();
      }
      break;

    case FR_CALIBRATE_JOYSTICKS:
      //up/ down here to select individual joysticks
      if(downBtn->updateEdge() == BE_FALLING)
      {
        setLearnThrottle();
      }

      if(upBtn->updateEdge() == BE_FALLING)
      {
        setLearnPoti();
      }

      //provide link to other frames
      if(leftBtn->updateEdge() == BE_FALLING)
      {
        setSendOffsets();
      }

      if(rightBtn->updateEdge() == BE_FALLING)
      {
        setStoreCalibration();
      }
      break;

    case FR_LEARN_THROTTLE:
      //up/ down here to select individual joysticks
      if(downBtn->updateEdge() == BE_FALLING)
      {
        setLearnRoll();
      }

      if(upBtn->updateEdge() == BE_FALLING)
      {
        setCalibrateJoysticks();
      }

      if(okBtn->updateEdge() == BE_FALLING)
      {
        setLearnThrottleMax();
      }
      break;

    case FR_LEARN_THROTTLE_MAX:
      //OK = store max and move on to min
      if (okBtn->updateEdge() == BE_FALLING)
      {
        (*controlInterface).learnThrottleJoyMax(); //learn max
        setLearnThrottleMin();
      }
      break;

    case FR_LEARN_THROTTLE_MIN:
      //OK = store min and return to choosing
      if (okBtn->updateEdge() == BE_FALLING)
      {
        (*controlInterface).learnThrottleJoyMin(); //learn min
        setLearnThrottle();
      }
      break;

    case FR_LEARN_ROLL:
      //up/ down here to select individual joysticks
      if(downBtn->updateEdge() == BE_FALLING)
      {
        setLearnPitch();
      }

      if(upBtn->updateEdge() == BE_FALLING)
      {
        setLearnThrottle();
      }

      if(okBtn->updateEdge() == BE_FALLING)
      {
        setLearnRollMax();
      }
      break;

    case FR_LEARN_ROLL_MAX:
      //OK = store max and move on to min
      if (okBtn->updateEdge() == BE_FALLING)
      {
        (*controlInterface).learnRollJoyMax(); //learn max
        setLearnRollMin();
      }
      break;

    case FR_LEARN_ROLL_MIN:
      //OK = store min and return to choosing
      if (okBtn->updateEdge() == BE_FALLING)
      {
        (*controlInterface).learnRollJoyMin(); //learn min
        setLearnRoll();
      }
      break;

    case FR_LEARN_PITCH:
      //up/ down here to select individual joysticks
      if(downBtn->updateEdge() == BE_FALLING)
      {
        setLearnYaw();
      }

      if(upBtn->updateEdge() == BE_FALLING)
      {
        setLearnRoll();
      }

      if(okBtn->updateEdge() == BE_FALLING)
      {
        setLearnPitchMax();
      }
      break;

    case FR_LEARN_PITCH_MAX:
      //OK = store max and move on to min
      if (okBtn->updateEdge() == BE_FALLING)
      {
        (*controlInterface).learnPitchJoyMax(); //learn max
        setLearnPitchMin();
      }
      break;

    case FR_LEARN_PITCH_MIN:
      //OK = store min and return to choosing
      if (okBtn->updateEdge() == BE_FALLING)
      {
        (*controlInterface).learnPitchJoyMin(); //learn min
        setLearnPitch();
      }
      break;

    case FR_LEARN_YAW:
      //up/ down here to select individual joysticks
      if(downBtn->updateEdge() == BE_FALLING)
      {
        setLearnPoti();
      }

      if(upBtn->updateEdge() == BE_FALLING)
      {
        setLearnPitch();
      }

      if(okBtn->updateEdge() == BE_FALLING)
      {
        setLearnYawMax();
      }
      break;

    case FR_LEARN_YAW_MAX:
      //OK = store max and move on to min
      if (okBtn->updateEdge() == BE_FALLING)
      {
        (*controlInterface).learnYawJoyMax(); //learn max
        setLearnYawMin();
      }
      break;

    case FR_LEARN_YAW_MIN:
      //OK = store min and return to choosing
      if (okBtn->updateEdge() == BE_FALLING)
      {
        (*controlInterface).learnYawJoyMin(); //learn min
        setLearnYaw();
      }
      break;

    case FR_LEARN_POTI:
      //up/ down here to select individual joysticks
      if(downBtn->updateEdge() == BE_FALLING)
      {
        setCalibrateJoysticks();
      }

      if(upBtn->updateEdge() == BE_FALLING)
      {
        setLearnYaw();
      }

      if(okBtn->updateEdge() == BE_FALLING)
      {
        setLearnPotiMax();
      }
      break;

    case FR_LEARN_POTI_MAX:
      //OK = store max and move on to min
      if (okBtn->updateEdge() == BE_FALLING)
      {
        (*controlInterface).learnPitAdjMax(); //learn max
        setLearnPotiMin();
      }
      break;

    case FR_LEARN_POTI_MIN:
      //OK = store min and return to choosing
      if (okBtn->updateEdge() == BE_FALLING)
      {
        (*controlInterface).learnPitAdjMin(); //learn min
        setLearnPoti();
      }
      break;

    case FR_STORE_CALIB:
      //on ok press, do the action
      if(okBtn->updateEdge() == BE_FALLING)
      {
        //write eeprom values
        EEPROM.put(PERSISTENTDATA_EEPROM_ADDR, globalData->persistent);
        lcd->setCursor(0, 1);
        lcd->print("   done!        ");
      }

      //provide link to other frames
      if(leftBtn->updateEdge() == BE_FALLING)
      {
        setCalibrateJoysticks();
      }

      if(rightBtn->updateEdge() == BE_FALLING)
      {
        setRestoreCalibration();
      }
      break;

    case FR_RESTORE_CALIB:
      //on ok press, do the action
      if(okBtn->updateEdge() == BE_FALLING)
      {
        //read eeprom values
        EEPROM.get(PERSISTENTDATA_EEPROM_ADDR, globalData->persistent);
        lcd->setCursor(0, 1);
        lcd->print("   done!        ");
      }

      //provide link to other frames
      if(leftBtn->updateEdge() == BE_FALLING)
      {
        setStoreCalibration();
      }

      if(rightBtn->updateEdge() == BE_FALLING)
      {
        setJoystickSettings();
      }
      break;
  }
}

void LCDMenu::displayNumberWithSign(uint8_t column, uint8_t line, int number, int digits)
{
  lcd->setCursor(column, line);
  if (number < 0)
  {
    lcd->print("-");
  } else {
    lcd->print("+");
  }
  displayNumber(column+1, line, number, digits);
}

void LCDMenu::displayNumber(uint8_t column, uint8_t line, int number, int digits)
{
  int tmp;
  char buf[2] = {'0','\0'}; //initialize a buffer with a string that contains one character ('0') + termination '\0'
  lcd->setCursor(column, line);
  for (uint8_t i = digits; i > 0; i--) //go from highest to lowest digit
  {
    tmp = abs(number);
    for (uint8_t j = 0; j < i-1; j++) //run divide the number down to the digit
    {
      tmp /= 10;
    }
    buf[0] = (char)(tmp % 10) + '0'; //clear of anything bigger 10 to get the single digit and add '0' to convert to ascii code
    lcd->print(buf);
  }
}

void LCDMenu::displayNumberWithSign(uint8_t column, uint8_t line, float number, int digitsBefore, int digitsAfter)
{
  lcd->setCursor(column, line);
  if (number < 0)
  {
    lcd->print("-");
  } else {
    lcd->print("+");
  }
  displayNumber(column+1, line, number, digitsBefore, digitsAfter);
}

void LCDMenu::displayNumber(uint8_t column, uint8_t line, float number, int digitsBefore, int digitsAfter)
{
  int numberBefore;
  int numberAfter;
  int mask;
  uint8_t i;

  mask = 1;
  for (i = 0; i < digitsBefore; i++) // make a mask to mask out preceeding digits
  {
    mask *= 10;
  }

  numberBefore = ((int)number) % mask;

  mask = 1;
  for (i = 0; i < digitsAfter; i++) // make a mask to mask out preceeding digits
  {
    mask *= 10;
  }

  numberAfter = ((int)(number * mask)) % mask; //shift digits up before masking

  displayNumber(column, line, numberBefore, digitsBefore);
  lcd->print(".");
  displayNumber(column + digitsBefore + 1, line, numberAfter, digitsAfter);
}

void LCDMenu::setHome( void )
{
  currentFrame = FR_HOME;
  lcd->clear();
  lcd->print("==== Home ======");
  lcd->setCursor(0, 1);
  lcd->print("press up/down");
}

void LCDMenu::setEnergyData( void )
{
  currentFrame = FR_ENERGY_DATA;
  lcd->clear();
  lcd->print("= Energy Data ==");
  lcd->setCursor(0, 1);
  lcd->print("  press < >");
}

void LCDMenu::setCells( void )
{
  currentFrame = FR_CELLS;
  lcd->clear();
  lcd->print("<Cell 1 2 3 [V]>");
  lcd->setCursor(0, 1);
  lcd->print("-.--  -.--  -.--");
}

void LCDMenu::setCurrent( void )
{
  currentFrame = FR_CURRENT;
  lcd->clear();
  lcd->print("< Current      >");
  lcd->setCursor(0, 1);
  lcd->print("  ----- mA ");
}

void LCDMenu::setCellsCurrent( void )
{
  currentFrame = FR_CELLS_CURRENT;
  lcd->clear();
  lcd->print("-.--  -.--  -.--");
  lcd->setCursor(0, 1);
  lcd->print("  ----- mA ");
}

void LCDMenu::setPower( void )
{
  currentFrame = FR_POWER;
  lcd->clear();
  lcd->print("< Power        >");
  lcd->setCursor(0, 1);
  lcd->print("  ---.--- W ");
}

void LCDMenu::setEnergy( void )
{
  currentFrame = FR_ENERGY;
  integral = 0.0;
  lcd->clear();
  lcd->print("Energy| OK=reset");
  lcd->setCursor(0, 1);
  lcd->print("  ---.--- Wh");
}

void LCDMenu::setCharge( void )
{
  currentFrame = FR_CHARGE;
  integral = 0.0;
  lcd->clear();
  lcd->print("Charge| OK=Reset");
  lcd->setCursor(0, 1);
  lcd->print("  ---.--- Ah");
}

void LCDMenu::setJoystickSettings( void )
{
  currentFrame = FR_JOSTICK_SETTINGS;
  lcd->clear();
  lcd->print("=Joyst-settings=");
  lcd->setCursor(0, 1);
  lcd->print("  press < >");
}

void LCDMenu::setSendOffsets( void )
{
  okBtn->updateEdge(); //update once to get latest reading of the button
  currentFrame = FR_SEND_OFFSETS;
  lcd->clear();
  lcd->print("< send offsets >");
  lcd->setCursor(0, 1);
  lcd->print("   press OK");
}

void LCDMenu::setCalibrateJoysticks( void )
{
  upBtn->updateEdge();
  downBtn->updateEdge();
  okBtn->updateEdge(); //update once to get latest reading of the button
  currentFrame = FR_CALIBRATE_JOYSTICKS;
  lcd->clear();
  lcd->print("<calibrate joys>");
  lcd->setCursor(0, 1);
  lcd->print(" press up/down  ");
}

void LCDMenu::setStoreCalibration( void )
{
  okBtn->updateEdge(); //update once to get latest reading of the button
  currentFrame = FR_STORE_CALIB;
  lcd->clear();
  lcd->print("<save cal data >");
  lcd->setCursor(0, 1);
  lcd->print("   press OK");
}

void LCDMenu::setRestoreCalibration( void )
{
  okBtn->updateEdge(); //update once to get latest reading of the button
  currentFrame = FR_RESTORE_CALIB;
  lcd->clear();
  lcd->print("< get cal data >");
  lcd->setCursor(0, 1);
  lcd->print("   press OK");
}

void LCDMenu::setLearnThrottle( void )
{
  upBtn->updateEdge();
  downBtn->updateEdge();
  okBtn->updateEdge(); //update once to get latest reading of the button
  currentFrame = FR_LEARN_THROTTLE;
  lcd->clear();
  lcd->print(" learn throttle ");
  lcd->setCursor(0, 1);
  lcd->print("   press OK");
}

void LCDMenu::setLearnThrottleMax( void )
{
  okBtn->updateEdge(); //update once to get latest reading of the button
  currentFrame = FR_LEARN_THROTTLE_MAX;
  lcd->clear();
  lcd->print(" learn thro max ");
  lcd->setCursor(0, 1);
  lcd->print("   press OK");
}

void LCDMenu::setLearnThrottleMin( void )
{
  okBtn->updateEdge(); //update once to get latest reading of the button
  currentFrame = FR_LEARN_THROTTLE_MIN;
  lcd->clear();
  lcd->print(" learn thro min ");
  lcd->setCursor(0, 1);
  lcd->print("   press OK");
}

void LCDMenu::setLearnRoll( void )
{
  upBtn->updateEdge();
  downBtn->updateEdge();
  okBtn->updateEdge(); //update once to get latest reading of the button
  currentFrame = FR_LEARN_ROLL;
  lcd->clear();
  lcd->print(" learn roll     ");
  lcd->setCursor(0, 1);
  lcd->print("   press OK");
}

void LCDMenu::setLearnRollMax( void )
{
  okBtn->updateEdge(); //update once to get latest reading of the button
  currentFrame = FR_LEARN_ROLL_MAX;
  lcd->clear();
  lcd->print(" learn roll max ");
  lcd->setCursor(0, 1);
  lcd->print("   press OK");
}

void LCDMenu::setLearnRollMin( void )
{
  okBtn->updateEdge(); //update once to get latest reading of the button
  currentFrame = FR_LEARN_ROLL_MIN;
  lcd->clear();
  lcd->print(" learn roll min ");
  lcd->setCursor(0, 1);
  lcd->print("   press OK");
}

void LCDMenu::setLearnPitch( void )
{
  upBtn->updateEdge();
  downBtn->updateEdge();
  okBtn->updateEdge(); //update once to get latest reading of the button
  currentFrame = FR_LEARN_PITCH;
  lcd->clear();
  lcd->print(" learn pitch    ");
  lcd->setCursor(0, 1);
  lcd->print("   press OK");
}

void LCDMenu::setLearnPitchMax( void )
{
  okBtn->updateEdge(); //update once to get latest reading of the button
  currentFrame = FR_LEARN_PITCH_MAX;
  lcd->clear();
  lcd->print("learn pitch max ");
  lcd->setCursor(0, 1);
  lcd->print("   press OK");
}

void LCDMenu::setLearnPitchMin( void )
{
  okBtn->updateEdge(); //update once to get latest reading of the button
  currentFrame = FR_LEARN_PITCH_MIN;
  lcd->clear();
  lcd->print("learn pitch min ");
  lcd->setCursor(0, 1);
  lcd->print("   press OK");
}


void LCDMenu::setLearnYaw( void )
{
  upBtn->updateEdge();
  downBtn->updateEdge();
  okBtn->updateEdge(); //update once to get latest reading of the button
  currentFrame = FR_LEARN_YAW;
  lcd->clear();
  lcd->print(" learn yaw      ");
  lcd->setCursor(0, 1);
  lcd->print("   press OK");
}

void LCDMenu::setLearnYawMax( void )
{
  okBtn->updateEdge(); //update once to get latest reading of the button
  currentFrame = FR_LEARN_YAW_MAX;
  lcd->clear();
  lcd->print(" learn yaw max  ");
  lcd->setCursor(0, 1);
  lcd->print("   press OK");
}

void LCDMenu::setLearnYawMin( void )
{
  okBtn->updateEdge(); //update once to get latest reading of the button
  currentFrame = FR_LEARN_YAW_MIN;
  lcd->clear();
  lcd->print(" learn yaw min  ");
  lcd->setCursor(0, 1);
  lcd->print("   press OK");
}

void LCDMenu::setLearnPoti( void )
{
  upBtn->updateEdge();
  downBtn->updateEdge();
  okBtn->updateEdge(); //update once to get latest reading of the button
  currentFrame = FR_LEARN_POTI;
  lcd->clear();
  lcd->print(" learn poti     ");
  lcd->setCursor(0, 1);
  lcd->print("   press OK");
}

void LCDMenu::setLearnPotiMax( void )
{
  okBtn->updateEdge(); //update once to get latest reading of the button
  currentFrame = FR_LEARN_POTI_MAX;
  lcd->clear();
  lcd->print(" learn poti max ");
  lcd->setCursor(0, 1);
  lcd->print("   press OK");
}

void LCDMenu::setLearnPotiMin( void )
{
  okBtn->updateEdge(); //update once to get latest reading of the button
  currentFrame = FR_LEARN_POTI_MIN;
  lcd->clear();
  lcd->print(" learn poti min ");
  lcd->setCursor(0, 1);
  lcd->print("   press OK");
}
