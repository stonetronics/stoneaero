///avrdude upload command: avrdude -c usbasp -p m328p -u -U flash:w:firmware.hex


//Include needed Libraries at beginning
#include <Arduino.h>
#include <Servo.h>
#include "nRF24L01.h" //NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"
#include "printf.h"

#include "pindef.h"
#include "servodef.h"
#include "Messages.h"
#include "Analog.h"
#include "Monoflop.h"

#define TIMEOUT_DESCENT   2000 //ms   timeout for going into emergency mode

Analog analog;

RF24 radio(CE, CSN); // NRF24L01 used SPI pins + Pin 9 and 10 on the NANO

Monoflop monoflopDescent;
Monoflop monoflopReset;

Servo motor;
Servo rudder;
Servo elevator;
Servo aileronRight;
Servo aileronLeft;

const uint64_t commandPipe = 1; // Needs to be the same for communicating between 2 NRF24L01
const uint64_t reportPipe = 2;

Command command;
Report report;

int16_t motorOffset = 0;
int16_t rudderOffset = 0;
int16_t elevatorOffset = 0;
int16_t aileronsOffset = 0;

void setup(void){

  Serial.begin(115200);
  printf_begin();
  Serial.println("Hullo! Dis eez d receevur!");

  monoflopDescent.setHoldTime(TIMEOUT_DESCENT); //time for emergency routine

  Serial.println("initializing Servos");

  //attach servos
  motor.attach(MOTOR_PIN);
  motor.write(MOTOR_MAX);
  rudder.attach(RUDDER_PIN);
  rudder.write(90);
  elevator.attach(ELEVATOR_PIN);
  elevator.write(90);
  aileronRight.attach(AILERONRIGHT_PIN);
  aileronRight.write(90);
  aileronLeft.attach(AILERONLEFT_PIN);
  aileronLeft.write(90);

  //init led
  pinMode(LED, OUTPUT);
  //blink it
  for (uint8_t i = 0; i < 5; i++)
  {
    digitalWrite(LED, LOW);
    delay(300);
    digitalWrite(LED, HIGH);
    delay(120);
  }
  digitalWrite(LED, LOW);

  Serial.println("Initializing Analog measurements");
  analog.setCellCalibration(0, 29, 6, 0);
  analog.setCellCalibration(1, 29, 6, 0);
  analog.setCellCalibration(2, 29, 6, 0);
  analog.setCurrentCalibration(11689,101,-59255);

  Serial.println("Initializing radio");

  radio.begin(); // Start the NRF24L01
  radio.openWritingPipe(reportPipe);
  radio.openReadingPipe(1, commandPipe); //get ready to receive
  radio.setDataRate( RF24_250KBPS ) ;
  radio.setPALevel( RF24_PA_MAX ) ;
  radio.setChannel(0x34);
  radio.enableDynamicPayloads() ;
  radio.enableAckPayload();               // not used here
  radio.setRetries(0, 15);                // Smallest time between retries, max no. of retries
  radio.setAutoAck( true ) ;
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
  radio.powerUp();
  radio.startListening();
}

void loop(void){

  if (!monoflopDescent.output()) //emergency - no transmitter signal received for TIMEOUT_DESCENT
  {
    Serial.println("Hmmst! Nopheeng receevhde. Eemurgance..");
    //cut motor
    motor.write(MOTOR_MIN); //todo: does the motor need throttle for safe descent? - probably
  }



  while (radio.available())
  {
    monoflopDescent.trigger();
    radio.read(&command, sizeof(command));
    switch (command.mode)
    {
      case 'c':
        if (command.controlByte & 0x01)
        { //flaps mode
          aileronRight.write(map(((command.roll) + aileronsOffset),-90,90,AILERONRIGHT_MIN,90));
          aileronLeft.write(map((-1)*((command.roll) + aileronsOffset),-90,90,AILERONLEFT_MIN,90));
        } else { //normal mode
          aileronRight.write(map(((command.roll) + aileronsOffset),-90,90,AILERONRIGHT_MIN,AILERONRIGHT_MAX));
          aileronLeft.write(map((-1)*((command.roll) + aileronsOffset),-90,90,AILERONLEFT_MIN,AILERONLEFT_MAX));
        }
        motor.write(map(((command.throttle) + motorOffset),0,100,MOTOR_MIN,MOTOR_MAX));
        rudder.write(map(((command.yaw) + rudderOffset),-90,90,RUDDER_MIN,RUDDER_MAX));
        elevator.write(map(((command.pitch) + elevatorOffset),-90,90,ELEVATOR_MIN,ELEVATOR_MAX));
        break;

      case 'o':
        //motorOffset = command.throttle;
        rudderOffset = command.yaw;
        elevatorOffset = command.pitch;
        aileronsOffset = command.roll;

      default:
        break;
    }

    //command received and processed - transmit report package
    report.current = analog.getCurrent();
    analog.getCells(&(report.cellVoltage[0]));
    radio.stopListening();
    radio.write(&report, sizeof(report)); // Send value through NRF24L01
    radio.startListening();
  }

}
