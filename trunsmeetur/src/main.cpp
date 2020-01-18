/*modify and use as you like*/


//Include needed Libraries at beginning
#include <Arduino.h>
#include <EEPROM.h>
#include "pindef.h"
#include "DataTypes.h"
#include "LiquidCrystal.h"
#include "LCDMenu.h"
#include "Button.h"
#include "Joystick.h"
#include "IdleSwitch.h"
#include "FingerSwitch.h"
#include "ControlInterface.h"
#include "Messages.h"
#include "nRF24L01.h" //NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"
#include "printf.h"
#include "CommunicationHandler.h"

#define VERBOSE   //uncomment to get verbose feedback on serial
//#define ANALOGAXIS_DEBUG  //uncomment to get all analog axis on output for calibration

//Instantiate Hardware
//Menu Buttons
Button okBtn(BTN_OK);
Button rightBtn(BTN_RIGHT);
Button leftBtn(BTN_LEFT);
Button downBtn(BTN_DOWN);
Button upBtn(BTN_UP);

//joystick offset buttons
Button lJoyR1IncBtn(LJOY_R1PLUS);
Button lJoyR1DecBtn(LJOY_R1MINUS);
Button lJoyR2IncBtn(LJOY_R2PLUS);
Button lJoyR2DecBtn(LJOY_R2MINUS);
Button rJoyR1IncBtn(RJOY_R1PLUS);
Button rJoyR1DecBtn(RJOY_R1MINUS);
Button rJoyR2IncBtn(RJOY_R2PLUS);
Button rJoyR2DecBtn(RJOY_R2MINUS);

//LCD display
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

//NRF24L01+ radio module
RF24 radio(CE, CSN);


//Shared Data
GlobalData globalData;

//instantiate software modules


int16_t lJoyYVal;
Joystick lJoyY(LJOY_R1, &lJoyR1IncBtn, &lJoyR1DecBtn, &(globalData.persistent.controlInterfaceConfiguration.throttleJoyConf), &lJoyYVal);
int16_t lJoyXVal;
Joystick lJoyX(LJOY_R2, &lJoyR2IncBtn, &lJoyR2DecBtn, &(globalData.persistent.controlInterfaceConfiguration.yawJoyConf), &lJoyXVal);
int16_t rJoyYVal;
Joystick rJoyY(RJOY_R1, &rJoyR1IncBtn, &rJoyR1DecBtn, &(globalData.persistent.controlInterfaceConfiguration.pitchJoyConf), &rJoyYVal);
int16_t rJoyXVal;
Joystick rJoyX(RJOY_R2, &rJoyR2IncBtn, &rJoyR2DecBtn, &(globalData.persistent.controlInterfaceConfiguration.rollJoyConf), &rJoyXVal);
IdleSwitchState swIdle; //state variable to store the switch value. it can be read from other modules to be used
IdleSwitch switchIdle(IDLE_1, IDLE_N, &swIdle);
uint8_t swElev;
FingerSwitch switchElev(ELEV_D_R, 0, &swElev); //dont invert switch
uint8_t swAile;
FingerSwitch switchAile(AILE_D_R, 0, &swAile);
uint8_t swThHold;
FingerSwitch switchThHold(TH_HOLD, 0, &swThHold);
uint8_t swGyroSens;
FingerSwitch switchGyroSens(GYRO_SENS, 0, &swGyroSens);
int16_t pitAdjVal;
Poti pitAdjPoti(PIT_ADJ, &(globalData.persistent.controlInterfaceConfiguration.pitAdjConf), &pitAdjVal);

ControlInterface controlInterface(&rJoyX, &rJoyY, &lJoyX, &lJoyY, &switchIdle, &switchElev, &switchAile, &switchThHold, &switchGyroSens, &pitAdjPoti);

LCDMenu menu(&lcd, &okBtn, &rightBtn, &leftBtn, &downBtn, &upBtn, &controlInterface, &globalData);

CommunicationHandler communicationHandler(&radio, &globalData);

void setup(void)
{
  //init global Data

  globalData.lastReport = {0, {0,0,0}};

  /* sample conf, uncomment this the first upload to write it into eeprom
  globalData.persistent.controlInterfaceConfiguration = {   {0, 1023, 0, -90, 90},
                                                            {0, 1023, 0, -90, 90},
                                                            {0, 1023, 0, -90, 90},
                                                            {0, 1023, 0, 0, 100},
                                                            {0, 1023, 0, 1023}  };
  //write eeprom values the first time
  EEPROM.put(PERSISTENTDATA_EEPROM_ADDR, globalData.persistent); */

  //set leds on pcb
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);
  digitalWrite(LED5, HIGH);
  digitalWrite(LED6, HIGH);

  //set case leds
  pinMode(S_POWER, OUTPUT);
  pinMode(S_CHARGE, OUTPUT);
  pinMode(S_HACK, OUTPUT);
  digitalWrite(S_POWER, HIGH);     //indicate power
  digitalWrite(S_CHARGE, LOW);
  digitalWrite(S_HACK, LOW);

  Serial.begin(115200);
  #ifdef VERBOSE
  Serial.println("ohw hullow! dis eez dee NRF24L01 trunsmeetur");
  #endif
  printf_begin();

  communicationHandler.initRadio();
  #ifdef VERBOSE
  communicationHandler.printRadioDetails();
  #endif
  digitalWrite(S_HACK, HIGH); //indicate radio startup

  //read eeprom values
  EEPROM.get(PERSISTENTDATA_EEPROM_ADDR, globalData.persistent);

  //first update of the conrol interface
  controlInterface.update();

}

void loop(void){
  ControlVector c;

  //Serial.println("loop");
  menu.update();
  controlInterface.update();
  communicationHandler.updateReport();
  c.thrust = lJoyYVal;
  c.roll = rJoyXVal;
  c.pitch = rJoyYVal;
  c.yaw = lJoyXVal;
  communicationHandler.sendCommand(generateControlCommand(c, swThHold));

  #ifdef ANALOGAXIS_DEBUG
  Serial.print("lY: ");
  Serial.print(lJoyYVal);
  Serial.print(" lYOffs: ");
  Serial.print(globalData.persistent.controlInterfaceConfiguration.throttleJoyConf.offset);
  Serial.print(" lX: ");
  Serial.print(lJoyXVal);
  Serial.print(" lXOffs: ");
  Serial.print(globalData.persistent.controlInterfaceConfiguration.yawJoyConf.offset);
  Serial.print(" rY: ");
  Serial.print(rJoyYVal);
  Serial.print(" rYOffs: ");
  Serial.print(globalData.persistent.controlInterfaceConfiguration.pitchJoyConf.offset);
  Serial.print(" rX: ");
  Serial.print(rJoyXVal);
  Serial.print(" rXOffs: ");
  Serial.print(globalData.persistent.controlInterfaceConfiguration.rollJoyConf.offset);
  Serial.print(" swIdle: ");
  Serial.print(swIdle);
  Serial.print(" swElev:");
  Serial.print(swElev);
  Serial.print(" swAile: ");
  Serial.print(swAile);
  Serial.print(" swThHold: ");
  Serial.print(swThHold);
  Serial.print(" swGyroSens: ");
  Serial.print(swGyroSens);
  Serial.print(" pitAdj: ");
  Serial.println(pitAdjVal);
  delay(200);
  #endif

}
