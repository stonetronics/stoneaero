#include <Arduino.h> //for integer definition
#include "pindef.h"

#define NO_CELLS  3

class Analog
{
  public:
    Analog();
    int16_t getCell(uint8_t cell); //cell 0 - 2; milliVolts
    void getCells(uint16_t* cells); //cells = address of cell1; milliVolts
    int16_t getCurrent( void ); // milliampere
    void setCellCalibration(uint8_t cell, int16_t k, int16_t j, int16_t d); //cell 0 - 2; V = k * ADC /j + d
    void setCurrentCalibration(int32_t k, int32_t j, int32_t d); //I = k * ADC / j + d
  private:
    int16_t cellK[NO_CELLS];
    int16_t cellJ[NO_CELLS];
    int16_t cellD[NO_CELLS];
    int32_t currentK;
    int32_t currentJ;
    int32_t currentD;
    int cellPins[NO_CELLS] = {VOLTAGE_CELL1, VOLTAGE_CELL2, VOLATGE_CELL3};
    int currentPin = CURRENTSENSE;
};
