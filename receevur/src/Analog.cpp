#include "Analog.h"

Analog::Analog()
{
  for (uint8_t i = 0; i < NO_CELLS; i++)
  {
    cellK[i] = 1;
    cellJ[i] = 1;
    cellD[i] = 0;
    analogRead(cellPins[i]);
  }
  currentK = 1;
  currentJ = 1;
  currentD = 0;
  analogRead(currentPin);
}

int16_t Analog::getCell(uint8_t cell)
{
  return cellK[cell] * analogRead(cellPins[cell]) / cellJ[cell] + cellD[cell];
}

void Analog::getCells(uint16_t* cells)
{
  for (uint8_t i = 0; i < NO_CELLS; i++)
  {
    cells[i] = Analog::getCell(i);
  }
}

int16_t Analog::getCurrent( void )
{
  return (int16_t)(((int32_t)(currentK)) * ((int32_t)(analogRead(currentPin))) / ((int32_t)(currentJ)) + ((int32_t)(currentD)));
}

void Analog::setCellCalibration(uint8_t cell, int16_t k, int16_t j, int16_t d)
{
  cellK[cell] = k;
  cellJ[cell] = j;
  cellD[cell] = d;
}
void Analog::setCurrentCalibration(int32_t k, int32_t j, int32_t d)
{
  currentK = k;
  currentJ = j;
  currentD = d;
}
