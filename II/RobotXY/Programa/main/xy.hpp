#ifndef _XY
#define _XY

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

/**********************************************/
//
//            CONSTANTES DEL SISTEMA
//
/**********************************************/

//Milimetros entre los extremos de los ejes.
#define mmX  2185
#define mmY  1783

//-------------------------
//        ENTRADAS Y SALIDAS ARDUINO
//-------------------------

#define O_MARCHA    42   //Enciende los Variadores
#define I_CMARCHA   32   //Motores activados

//Joystick
#define I_JKARR     30   //Arriba Joystick
#define I_JKABA     31   //Abajo Joystick
#define I_JKIZQ     25   //Izquierda Joystick
#define I_JKDER     26   //Derecha Joystick

//-------------------------
//        Eje X
//-------------------------

//Direcciones

#define O_STFX 46   //Avance dcha
#define O_STRX 47   //Avance izda

//Velocidades del Variador

#define O_RLX 37   //Baja
#define O_RMX 36   //Media
#define O_RHX 35   //Alta

//Finales de carrera

#define I_FCX1  23  //Izquierdo mirando de frente
#define I_FCX2  24  //Derecho mirando de frente

//Alarma

#define I_ALARMAX   22

//Comprobacion funcionamiento

#define I_RUNX  33

//Piloto estado eje

#define O_PAX   44

//-------------------------
//        Eje Y
//-------------------------

//Direcciones

#define O_STFY 48   //Arriba
#define O_STRY 49   //Abajo

//Velocidades

#define O_RLY 41   //Baja
#define O_RMY 40   //Media
#define O_RHY 39   //Alta

//Finales de carrera

#define I_FCY1  28  //Arriba
#define I_FCY2  29  //Abajo

//Alarma

#define I_ALARMAY   27

//Comprobacion funcionamiento

#define I_RUNY  34

//Freno
#define O_FRENO 43 //Normalmente cerrado

//Piloto estado eje

#define O_PAY 45

/**********************************************/
//
//      FUNCIONES DE INTERRUPCION
//
/**********************************************/

//Interrupciones de los pines
#define INT_EMERG 18
#define INT_ENX 2
#define INT_ENY 3
#define INT_INTFINC 19

/**********************************************/
//
//            ESTADOS DEL SISTEMA
//
/**********************************************/
#define DIRBACK 0
#define DIRFORWARD 1
#define FRENODES 1 //NORMALMENTE CERRADO
#define FRENOACT 0

unsigned int contadorPasosX = 0;
unsigned int contadorPasosY = 0;

bool dirMotorX = false; // False -> LEFT; True -> RIGHT
bool dirMotorY = false; // False -> DOWN; True -> UP

bool freno = false;

bool calibrateMode = false;

void intEncoderX();
void intEncoderY();
void intEmergency();
void intActivateEndstop();
void motorHome(unsigned short motorSelect);
void activeMotors();
void desactiveMotors();
void moveLeft();
void moveRight();
void moveUp();
void moveDown();
void stopMotorX();
void stopMotorY();

void moveLeft()
{
  digitalWrite(O_STFX,LOW);
  digitalWrite(O_STRX,HIGH);
  dirMotorX = false;
}

void moveRight()
{
  digitalWrite(O_STRX,LOW);
  digitalWrite(O_STFX,HIGH);
  dirMotorX = true;
}

void moveUp()
{
  digitalWrite(O_STRY,LOW);
  digitalWrite(O_STFY,HIGH);
  dirMotorY = true;
}

void moveDown()
{
  digitalWrite(O_STFY,LOW);
  digitalWrite(O_STRY,HIGH);
  dirMotorY = false;
}

void stopMotorX()
{
  digitalWrite(O_STFX,LOW);
  digitalWrite(O_STRX,LOW);
}

void stopMotorY()
{
  digitalWrite(O_STFY,LOW);
  digitalWrite(O_STRY,LOW);
}

void initPinModes()
{
  // INPUT
  pinMode(I_CMARCHA, INPUT);
  pinMode(I_JKARR, INPUT);  
  pinMode(I_JKABA,INPUT);
  pinMode(I_JKIZQ,INPUT);
  pinMode(I_JKDER,INPUT);
  pinMode(I_FCX1,INPUT);
  pinMode(I_FCX2,INPUT);
  pinMode(I_ALARMAX,INPUT);
  pinMode(I_RUNX,INPUT);
  pinMode(I_FCY1,INPUT);
  pinMode(I_FCY2,INPUT);
  pinMode(I_ALARMAY,INPUT);
  pinMode(I_RUNY,INPUT);

  
  //OUTPUT
  pinMode(O_MARCHA, OUTPUT);
  pinMode(O_STFX,OUTPUT);
  pinMode(O_STFY,OUTPUT);
  pinMode(O_RLX,OUTPUT);
  pinMode(O_RMX,OUTPUT);
  pinMode(O_RHX,OUTPUT);
  pinMode(O_PAX,OUTPUT);
  pinMode(O_STFY,OUTPUT);
  pinMode(O_STRY,OUTPUT);
  pinMode(O_RLY,OUTPUT);
  pinMode(O_RMY,OUTPUT);
  pinMode(O_RHY,OUTPUT);
  pinMode(O_FRENO,OUTPUT);
  pinMode(O_PAY,OUTPUT);


  //INTERRUPTS
  //TODO CHANGE THE INTERRUPTION MODE.
  attachInterrupt(0,intEncoderX,RISING);
  attachInterrupt(1,intEncoderY,RISING);
  attachInterrupt(4,intActivateEndstop,RISING);
  attachInterrupt(5,intEmergency,RISING);
}



void activeMotors()
{
  digitalWrite(O_MARCHA,HIGH);
}

void desactiveMotors()
{
  digitalWrite(O_MARCHA,LOW);
}

//vel = 1 -> LOW, vel = 2 -> MEDIUM, vel = 3 -> HIGH
void selectVelocityX(unsigned short velocity)
{
  digitalWrite(O_RLX,LOW);
  digitalWrite(O_RMX,LOW);
  digitalWrite(O_RHX,LOW);
      
  if(velocity == 1)
    digitalWrite(O_RLX,HIGH);
  else if(velocity == 2)
    digitalWrite(O_RMX,HIGH);
  else if(velocity == 3)
    digitalWrite(O_RHX,HIGH);
  else
    Serial.println("ERROR: Select Velocity > 3");
}

//vel = 1 -> LOW, vel = 2 -> MEDIUM, vel = 3 -> HIGH
void selectVelocityY(unsigned short velocity)
{
  digitalWrite(O_RLY,LOW);
  digitalWrite(O_RMY,LOW);
  digitalWrite(O_RHY,LOW);
      
  if(velocity == 1)
    digitalWrite(O_RLY,HIGH);
  else if(velocity == 2)
    digitalWrite(O_RMY,HIGH);
  else if(velocity == 3)
    digitalWrite(O_RHY,HIGH);
  else
    Serial.println("ERROR: Select Velocity > 3");
}


// motorSelect = 0 -> X ; motorSelect = 1 -> Y
void motorHome(unsigned short motorSelect)
{
  calibrateMode = true;
  
  if(motorSelect == 0)
    {//Motor X
      if(digitalRead(I_CMARCHA) == HIGH)
        {
          selectVelocityX(1);
          stopMotorY();
          moveLeft();
          
        }
      else
        Serial.println("ERROR: The motorSelect is not active");
      
    }
  else if(motorSelect == 1)
    {// Motor Y
      
    }
  else
    {//UNKNOWN Motor
      Serial.println("ERROR: motorSelect > 1");
      calibrateMode = false;
      intEmergency();
    }
    
  calibrateMode = false;
}

void intEncoderX()
{
  if (!calibrateMode)
    {
      if(dirMotorX == DIRBACK)
        contadorPasosX--;
      else
        contadorPasosX++;
    }
}

void intEncoderY()
{
  if(!calibrateMode)
    {
      if(dirMotorY == DIRBACK)
        contadorPasosY--;
      else
        contadorPasosY++;
    }
}

void intActivateEndstop()
{
  if(digitalRead(I_FCX1) || digitalRead(I_FCX2))
    {
      stopMotorX();
    }
  else if (digitalRead(I_FCY1) || digitalRead(I_FCX2))
    {
      stopMotorY();
    }
  else
    {
      stopMotorY();
      stopMotorX();
    }
}

void intEmergency()
{
  //TODO
}



#endif
