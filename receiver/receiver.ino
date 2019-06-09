
//Include needed Libraries at beginning
#include <Servo.h>
#include "nRF24L01.h" //NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"

#include "Monoflop.h"

#define MOTOR_MAX     180
#define MOTOR_MIN     10

#define RUDDER_MAX    130
#define RUDDER_MIN    50

#define ELEVATOR_MAX  130
#define ELEVATOR_MIN  50

#define AILERON_MAX   130
#define AILERON_MIN   50

#define MONOFLOP_HOLDTIME   1500 //ms

int8_t message[5];

RF24 radio(9,10); // NRF24L01 used SPI pins + Pin 9 and 10 on the NANO

Monoflop monoflop;

Servo motor;
Servo rudder;
Servo elevator;
Servo ailerons;

const uint64_t pipe = 1; // Needs to be the same for communicating between 2 NRF24L01 

int16_t motorOffset = 0;
int16_t rudderOffset = 0;
int16_t elevatorOffset = 0;
int16_t aileronsOffset = 0;


void setup(void){

  monoflop.setHoldTime(MONOFLOP_HOLDTIME); //1500ms time for emergency routine

  motor.attach(8);
  motor.write(MOTOR_MAX);
  rudder.attach(7);
  rudder.write(90);
  elevator.attach(6);
  elevator.write(90);
  ailerons.attach(5);
  ailerons.write(90);


  
  
  radio.begin(); // Start the NRF24L01
  radio.openReadingPipe(1,pipe); //get ready to receive
  radio.startListening();//listen, here
}

void loop(void){

  if (!monoflop.output()) //emergency - no transmitter signal received for MONOFLOP_HOLDTIME
  {
    motor.write(MOTOR_MIN); //todo: does the motor need throttle for safe descent?
    elevator.write(90);
    rudder.write(70);
    ailerons.write(85); //circular even descent
  }
  

  while (radio.available())
  {
    monoflop.trigger();
    radio.read(message, 5);
    switch (message[0])
    {
      case 'c':
        motor.write(map((((int16_t)message[1]) + motorOffset),-90,90,MOTOR_MIN,MOTOR_MAX));
        rudder.write(map((((int16_t)message[2]) + rudderOffset),-90,90,RUDDER_MIN,RUDDER_MAX));
        elevator.write(map((((int16_t)message[3]) + elevatorOffset),-90,90,ELEVATOR_MIN,ELEVATOR_MAX));
        ailerons.write(map((((int16_t)message[4]) + aileronsOffset),-90,90,AILERON_MIN,AILERON_MAX));
        break;

      case 'o':
        //motorOffset = message[1];
        rudderOffset = message[2];
        elevatorOffset = message[3];
        aileronsOffset = message[4];

      default:
        break;
    }
  }
}
