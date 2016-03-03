#ifndef __firmdataXY__
#define __firmdataXY__
#include "xy.hpp"


void initFirmdata(unsigned short baudrate)
{
  Serial.begin(baudrate);
}

void manageOrder()
{
  while (true)
    {
      Serial.println("HOla");
    }
}



#endif
