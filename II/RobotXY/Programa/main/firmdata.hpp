#ifndef __firmdataXY__
#define __firmdataXY__
#include "xy.hpp"


void initFirmdata(unsigned short baudrate)
{
  Serial.begin(baudrate);
}

void manageOrder()
{
  byte controller = 0;
  byte controller2 = 0;
  short atributos = 0;
  unsigned short posicion = 0;
  
  Serial.println("Entrada en el modo control remoto.");
  
  while (controller != 110)
    {
      if(Serial.available())
        {
          posicion = 0;
          while(Serial.available() > 0)
            {
              delay(10);
              if(posicion == 0)
                controller = Serial.read();
              else if (posicion = 1)
                controller2 = Serial.read();
              else
                {
                  atributos *= 10;
                  atributos += Serial.read() -48;
                  
                }
              posicion++;
            }
          posicion = 0;
          Serial.print("He recibido: ");
          Serial.print(controller);
          Serial.print(controller2);
          Serial.println(atributos);
        }

      //TODO Poner modulos que acceden segun el controlador.
      
    }
  Serial.println("Salida del modo control remoto.");
}



#endif
