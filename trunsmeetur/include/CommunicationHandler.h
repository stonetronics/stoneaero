#ifndef __COMMUNICATIONHANDLER_H__
#define __COMMUNICATIONHANDLER_H__

#include <Arduino.h>
#include "DataTypes.h"
#include "Messages.h"
#include "nRF24L01.h" //NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"

Command generateControlCommand(ControlVector v, uint8_t airBrakes);

class CommunicationHandler
{
  public:
    CommunicationHandler( RF24* radio, GlobalData* globalData);
    void initRadio ( void );
    void printRadioDetails ( void );
    void sendCommand ( Command command );
    void updateReport ( void ); //update lastReport in globalData
  private:
    RF24* radio;
    GlobalData* globalData;
};


#endif
