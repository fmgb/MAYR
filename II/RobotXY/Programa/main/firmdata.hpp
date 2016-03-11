#ifndef __firmdataXY__
#define __firmdataXY__
#include "xy.hpp"

byte controller = 0;
byte controller2 = 0;
short atributos = 0;
unsigned short posicion = 0;

void initFirmdata(unsigned short baudrate)
{
  Serial.begin(baudrate);
}

bool messageReceived()
{
  if(Serial.available())
    {
      controller = 0;
      controller2 = 0;
      atributos = 0;
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
      return true;
    }
  return false;
}


void manageOrder() 
{
  Serial.println("Entrada en el modo control remoto.");
  
  while (controller != 110) // Para salir de este modo, enviar 'n'.
    {
      if(messageReceived())
        {
          if(controller == 88) // X -> 88
            {
              if(controller2 == 80) // P -> 80
                {
                  moveXSteps(atributos);
                }
              else if(controller2 == 77) // M -> 77
                {
                  moveXmm(atributos);
                }
              else if(controller2 == 86)
                selectVelocityX(atributos);
              else
                Serial.println("ERROR: Verifique la API");
            }

            if(controller == 89) // Y -> 89
              {
                if(controller2 == 80) // P -> 80
                {
                  moveYSteps(atributos);
                }
              else if(controller2 == 77) // M -> 77
                {
                  moveYmm(atributos);
                }
              else if(controller2 == 86)
                selectVelocityY(atributos);
                
              else
                Serial.println("ERROR: Verifique la API");
              }
            if(controller == 70)
              {
                if(controller2 == 67)
                  Serial.println(getStateEndStops());
                else
                  Serial.println("ERROR: Verifique la API");
              }
            if(controller == 69)
              {
                if(controller2 == 77)
                  interruptEmergency();
              }
            if(controller == 65)
              {
                if(controller2 == 70)
                  activeBrake();
              }
            if(controller == 68)
              {
                if(controller2 == 70)
                  deactiveBrake();
              }
        }
      
    }
  Serial.println("Salida del modo control remoto.");
}



#endif
