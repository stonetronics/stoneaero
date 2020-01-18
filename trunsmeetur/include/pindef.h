#ifndef __PINDEF_H__
#define __PINDEF_H__


//JOYSTICKS

#define LJOY_R1       A13
#define LJOY_R1PLUS   41
#define LJOY_R1MINUS  40

#define LJOY_R2       A12
#define LJOY_R2PLUS   43
#define LJOY_R2MINUS  42

#define RJOY_R1       A15
#define RJOY_R1PLUS   47
#define RJOY_R1MINUS  46

#define RJOY_R2       A14
#define RJOY_R2PLUS   49
#define RJOY_R2MINUS  48


//PCB BUTTONS

#define BTN1    12
#define BTN2    11
#define BTN3    10
#define BTN4    9
#define BTN5    13

//readability
#define BTN_UP      BTN4
#define BTN_DOWN    BTN3
#define BTN_LEFT    BTN2
#define BTN_RIGHT   BTN1
#define BTN_OK      BTN5


//PCB LEDS (0-active)

#define LED1  19
#define LED2  18
#define LED3  17
#define LED4  16
#define LED5  15
#define LED6  14


//pitch adj poti

#define PIT_ADJ   A11


//finger switches

#define GYRO_SENS   (22)
#define ELEV_D_R    (23)
#define IDLE_N      (24)
#define IDLE_1      (25)
#define AILE_D_R    (26)
#define TH_HOLD     (27)


//case LEDS (1-active)

#define S_LED1    30
#define S_LED2    29
#define S_LED3    28

//readability
#define S_CHARGE  S_LED2
#define S_HACK    S_LED3
#define S_POWER   S_LED1

//nRF24L01+PA+LNA connection

#define CE      44
#define CSN     45


//MPU9250 interrupt line

#define INT     2


//LCD connection

#define LCD_RS    3
#define LCD_E     4
#define LCD_D4    5
#define LCD_D5    6
#define LCD_D6    7
#define LCD_D7    8


#endif
