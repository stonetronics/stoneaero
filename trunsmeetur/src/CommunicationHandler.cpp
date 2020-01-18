#include "CommunicationHandler.h"

Command generateControlCommand(ControlVector v, uint8_t airBrakes)
{
  Command cmd;
  cmd.mode = 'c'; //command mode
  cmd.throttle = v.thrust;
  cmd.roll = v.roll;
  cmd.pitch = v.pitch;
  cmd.yaw = v.yaw;
  if (airBrakes)
  {
    cmd.controlByte = 0x01;
  } else {
    cmd.controlByte = 0x00;
  }
  return cmd;
}

CommunicationHandler::CommunicationHandler(RF24* radio, GlobalData* globalData)
{
  this->radio = radio;
  this->globalData = globalData;
}

void CommunicationHandler::initRadio( void )
{
  radio->begin(); // Start the NRF24L01
  radio->openWritingPipe(COMMANDPIPE); // Get NRF24L01 ready to transmit
  radio->openReadingPipe(1, REPORTPIPE);
  radio->setDataRate( RF24_250KBPS ) ;
  radio->setPALevel( RF24_PA_MAX ) ;
  radio->setChannel(0x34);
  radio->enableDynamicPayloads() ;
  radio->enableAckPayload();               // not used here
  radio->setRetries(0, 15);                // Smallest time between retries, max no. of retries
  radio->setAutoAck( true ) ;
  radio->powerUp();
  radio->startListening();
}

void CommunicationHandler::printRadioDetails( void )
{
  radio->printDetails();                   // Dump the configuration of the rf unit for debugging
}

void CommunicationHandler::sendCommand(Command command)
{
  radio->stopListening();
  radio->write(&command, sizeof(command)); // Send value through NRF24L01
  radio->startListening();
}

void CommunicationHandler::updateReport( void )
{
  if (radio->available())
  {
    while (radio->available())
    {
      radio->read(&(globalData->lastReport), sizeof(globalData->lastReport));
    }
  }
}
