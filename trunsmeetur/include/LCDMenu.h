#include <Arduino.h>
#include "DataTypes.h"
#include "LiquidCrystal.h"
#include "Button.h"
#include "ControlInterface.h"

typedef enum {
  FR_START,
  FR_HOME,
  FR_ENERGY_DATA, FR_CELLS, FR_CURRENT, FR_CELLS_CURRENT, FR_POWER, FR_ENERGY, FR_CHARGE,
  FR_JOSTICK_SETTINGS, FR_SEND_OFFSETS, FR_CALIBRATE_JOYSTICKS, FR_STORE_CALIB, FR_RESTORE_CALIB,
  FR_LEARN_THROTTLE, FR_LEARN_THROTTLE_MAX, FR_LEARN_THROTTLE_MIN, FR_LEARN_ROLL, FR_LEARN_ROLL_MAX, FR_LEARN_ROLL_MIN, FR_LEARN_PITCH, FR_LEARN_PITCH_MAX, FR_LEARN_PITCH_MIN, FR_LEARN_YAW, FR_LEARN_YAW_MAX, FR_LEARN_YAW_MIN, FR_LEARN_POTI, FR_LEARN_POTI_MAX, FR_LEARN_POTI_MIN
} Frame;

class LCDMenu
{
  public:
    //the constructor takes an instantiated lcd and Buttons to work with, also give the control interface to set the max/min, global data for data operations
    LCDMenu( LiquidCrystal* lcd , Button* okBtn, Button* rightBtn, Button* leftBtn, Button* downBtn, Button* upBtn, ControlInterface* controlInterface, GlobalData* globalData);
    void update( void );
  private:
    void displayNumber(uint8_t column, uint8_t line, int number, int digits);
    void displayNumber(uint8_t column, uint8_t line, float number, int digitsBefore, int digitsAfter);
    void displayNumberWithSign(uint8_t column, uint8_t line, int number, int digits);
    void displayNumberWithSign(uint8_t column, uint8_t line, float number, int digitsBefore, int digitsAfter);
    GlobalData* globalData;
    ControlInterface* controlInterface;
    LiquidCrystal* lcd;
    Button* okBtn;
    Button* rightBtn;
    Button* leftBtn;
    Button* downBtn;
    Button* upBtn;

    Frame currentFrame;

    int millisPrev;
    float integral; //for energy/charge display

    //handler functions for each state to set the screen to it (except start screen which is displayed by calling the constructor)
    void setHome(void);
    void setEnergyData(void);
    void setCells(void);
    void setCurrent(void);
    void setCellsCurrent(void);
    void setPower(void);
    void setEnergy(void);
    void setCharge(void);
    void setJoystickSettings(void);
    void setSendOffsets(void);
    void setCalibrateJoysticks(void);
    void setStoreCalibration(void);
    void setRestoreCalibration(void);
    void setLearnThrottle(void);
    void setLearnThrottleMax(void);
    void setLearnThrottleMin(void);
    void setLearnRoll(void);
    void setLearnRollMax(void);
    void setLearnRollMin(void);
    void setLearnPitch(void);
    void setLearnPitchMax(void);
    void setLearnPitchMin(void);
    void setLearnYaw(void);
    void setLearnYawMax(void);
    void setLearnYawMin(void);
    void setLearnPoti(void);
    void setLearnPotiMax(void);
    void setLearnPotiMin(void);
};
