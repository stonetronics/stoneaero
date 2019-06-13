/*modify and use as you like*/


//Include needed Libraries at beginning
#include "nRF24L01.h" //NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"
#include "AnalogInterfaces.h"
#include "RateSweeper.h"

#define SwitchPin 7 // button is connected to Pin 8 on NANO

#define BTN_1   5
#define BTN_2   6
#define BTN_3   7
#define BTN_4   8

int8_t message[5]; // Used to store value before being sent through the NRF24L01

RateSweeper rudderRate;
RateSweeper elevatorRate;
RateSweeper aileronRate;

AnalogInterfaces analogInterfaces; //init analog interfaces
RF24 radio(9,10); // NRF24L01 used SPI pins + Pin 9 and 10 on the NANO

const uint64_t pipe = 1; // Needs to be the same for communicating between 2 NRF24L01 

uint8_t btn1, btn1_old;
uint8_t btn2, btn2_old;
uint8_t btn3, btn3_old;
uint8_t btn4, btn4_old;

void setup(void){
  
  Serial.begin(9600);
  Serial.println("NRF24L01 test transmitter");

  pinMode(BTN_1, INPUT);  //buttons are inputs
  btn1 = btn1_old = digitalRead(BTN_1); //init edge detection
  pinMode(BTN_2, INPUT);
  btn2 = btn2_old = digitalRead(BTN_2);
  pinMode(BTN_3, INPUT);
  btn3 = btn3_old = digitalRead(BTN_3);
  pinMode(BTN_4, INPUT);
  btn4 = btn4_old = digitalRead(BTN_4);
  
  pinMode(SwitchPin, INPUT_PULLUP); // Define the arcade switch NANO pin as an Input using Internal Pullups
  digitalWrite(SwitchPin,HIGH); // Set Pin to HIGH at beginning
  
  radio.begin(); // Start the NRF24L01
  radio.openWritingPipe(pipe); // Get NRF24L01 ready to transmit

  rudderRate.setRate(2);
  elevatorRate.setRate(3);
  aileronRate.setRate(4);


  //analogInterfaces.retrieveExtremes(); //load extreme values for the analog interface from eeprom, to initialize
}

void loop(void){
  btn1 = digitalRead(BTN_1);
  btn2 = digitalRead(BTN_2);
  btn3 = digitalRead(BTN_3);
  btn4 = digitalRead(BTN_4);

  if (btn1 != btn1_old) //check button1
  {
    btn1_old = btn1;
    if (!btn1) //on press, send store offset command
    {
      message[0] = 'o';
      message[1] = analogInterfaces.getPotiAServoScale(); //motor
      message[2] = analogInterfaces.getLX() * rudderRate.getRate() / 4; //rudder
      message[3] = analogInterfaces.getRY() * elevatorRate.getRate() / 4; //elevator
      message[4] = analogInterfaces.getRX() * aileronRate.getRate() / 4; //ailerons
      
      radio.write(message, 5); // Send value through NRF24L01
    }
  }

  if (btn2 != btn2_old)
  {
    btn2_old = btn2;
    if (!btn2)
    {
      rudderRate.nextRate();
    }
  }

  if (btn3 != btn3_old)
  {
    btn3_old = btn3;
    if (!btn3)
    {
      elevatorRate.nextRate();
    }
  }

  if (btn4 != btn4_old)
  {
    btn4_old = btn4;
    if (!btn4)
    {
      aileronRate.nextRate();
    }
  }
  
  message[0] = 'c';
  message[1] = analogInterfaces.getPotiAServoScale();
  message[2] = analogInterfaces.getLX() * rudderRate.getRate() / 4; //rudder
  message[3] = analogInterfaces.getRY() * elevatorRate.getRate() / 4; //elevator
  message[4] = analogInterfaces.getRX() * aileronRate.getRate() / 4; //ailerons
  
  radio.write(message, 5); // Send value through NRF24L01

}
