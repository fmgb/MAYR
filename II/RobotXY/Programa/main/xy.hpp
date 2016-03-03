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

int contadorPasosX = 0;
int contadorPasosY = 0;

float stepsX = 0;
float stepsY = 0;

bool dirMotorX = false; // False -> LEFT; True -> RIGHT
bool dirMotorY = false; // False -> DOWN; True -> UP

bool freno = false;

void intEncoderX();
void intEncoderY();
void intEmergency();
void intActivateEndstop();
void motorHome(unsigned short motorSelect); // 1 -> X, 2 -> Y
void activeMotors();
void desactiveMotors();
void moveLeft();
void moveRight();
void moveUp();
void moveDown();
void stopMotorX();
void stopMotorY();
void activeBrake();
void deactiveBrake();
void controlJostick();
void calibrate(); // Steps per mm.
void moveXmm(int mm); // + -> Right, - -> Left
void moveYmm(int mm); // + -> Up, - -> Down
unsigned short getStateEndStops();

void moveLeft()
{
  digitalWrite(O_STFX,LOW);
  digitalWrite(O_STRX,HIGH);
  digitalWrite(O_PAX,HIGH);
  dirMotorX = false;
}

void moveRight()
{
  digitalWrite(O_STRX,LOW);
  digitalWrite(O_STFX,HIGH);
  digitalWrite(O_PAX,HIGH);
  dirMotorX = true;
}

void moveUp()
{
  digitalWrite(O_STRY,LOW);
  digitalWrite(O_STFY,HIGH);
  digitalWrite(O_PAY,HIGH);
  dirMotorY = true;
}

void moveDown()
{
  digitalWrite(O_STFY,LOW);
  digitalWrite(O_STRY,HIGH);
  digitalWrite(O_PAY,HIGH);
  dirMotorY = false;
}

void moveXmm(int mm)
{
  stopMotorY();
  stopMotorX();

  if(mm > 0)
    {
      int contadorTarget = contadorPasosX + stepsX * mm;
      moveRight();
      while(contadorTarget > contadorPasosX) { };
      
      stopMotorX();
    }
  else if (mm < 0)
    {
      int contadorTarget = contadorPasosX + stepsX * mm;
      moveLeft();
      while(contadorTarget < contadorPasosX) { };
      stopMotorX();
    }
  else
    Serial.println("Move 0 mm");
}

void moveYmm(int mm)
{
  stopMotorY();
  stopMotorX();

  if(mm > 0)
    {
      int contadorTarget = contadorPasosY + stepsY * mm;
      moveUp();
      while(contadorTarget > contadorPasosY) { };
      stopMotorY();
    }
  else if (mm < 0)
    {
      int contadorTarget = contadorPasosY + stepsY * mm;
      moveDown();
      while(contadorTarget < contadorPasosY) { };
      stopMotorY();
    }
  else
    Serial.println("Move 0 mm");
}

void moveXSteps(int atributos)
{

}

void moveYSteps(int atributos)
{

}

void stopMotorX()
{
  digitalWrite(O_STFX,LOW);
  digitalWrite(O_STRX,LOW);
  digitalWrite(O_PAX,LOW);
}

void stopMotorY()
{
  activeBrake();
  digitalWrite(O_STFY,LOW);
  digitalWrite(O_STRY,LOW);
  digitalWrite(O_PAY,LOW);
}

void activeBrake()
{
  digitalWrite(O_FRENO, FRENOACT);
  freno = true;
}

void deactiveBrake()
{
  digitalWrite(O_FRENO, FRENODES);
  freno = false;
}

void initPinModes(unsigned short baudrate)
{
  Serial.begin(baudrate);
  
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

void deactiveMotors()
{
  activeBrake();
  
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
  if(digitalRead(I_CMARCHA) == HIGH)
    {
      if(motorSelect == 0)
        {//Motor X
          selectVelocityX(1);
          stopMotorY();
          moveLeft();
          while(!digitalRead(I_FCX1)){ };
          
          contadorPasosX = 0;
          moveRight();
          while(digitalRead(I_FCX1)) {};
          
          stopMotorX();
        }
      else if(motorSelect == 1)
        {// Motor Y
          selectVelocityY(1);
          stopMotorX();
          deactiveBrake();
          moveDown();
          while(!digitalRead(I_FCY2)) { };
          
          contadorPasosY = 0;
          moveUp();
          while(digitalRead(I_FCY2)){ };
          
          stopMotorY();
        }
      else
        {//UNKNOWN Motor
          Serial.println("ERROR: motorSelect > 1");
          intEmergency();
        }
    }
  else
    Serial.println("ERROR: The motorSelect is not active");
}

// Steps expresed in mm.
void calibrate(unsigned short motor)
{
  //Start [0,0]
  if(motor == 0)
    {
      moveRight();
      while(!digitalRead(I_FCX2)) { };
      
      int stepsXAux = contadorPasosX;
      Serial.print("\nSteps motor X per");
      Serial.print(mmX);
      Serial.print(": ");
      Serial.println(stepsX);
      moveLeft();
      while(digitalRead(I_FCX2)){};
      stopMotorX();
      
      stepsX = (stepsXAux)/((float)mmX);
    }
  else if(motor == 1)
    {
      moveUp();
      while(!digitalRead(I_FCY1)) { };
      
      int stepsYAux = contadorPasosY;
      Serial.print("\nSteps per");
      Serial.print(mmY);
      Serial.print(": ");
      Serial.println(stepsY);
      moveDown();
      while(digitalRead(I_FCY1)){};
      
      stopMotorY();
      
      stepsY = (stepsYAux)/((float) mmY);
    }
  else
    Serial.println("ERROR: Calibrate motor");
}

void intEncoderX()
{
  if(dirMotorX == DIRBACK)
    contadorPasosX--;
  else
        contadorPasosX++;
}

void intEncoderY()
{
  if(dirMotorY == DIRBACK)
    contadorPasosY--;
  else
    contadorPasosY++;
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
  stopMotorX();
  stopMotorY();
  deactiveMotors();
}

unsigned short getStateEndStops()
{
  unsigned short endStopAct = 0000;
  if(digitalRead(I_FCX1) == HIGH)
    endStopAct += 1000;
  if(digitalRead(I_FCX2) == HIGH)
    endStopAct += 0100;
  if(digitalRead(I_FCY1) == HIGH)
    endStopAct += 0010;
  if(digitalRead(I_FCY2) == HIGH)
    endStopAct += 0001;
  return endStopAct;
}

void controlJostick()
{
  byte controller = 0;

  Serial.println("Entrada en el modo Jostick");
  
  while(controller != 110)
    {
      if(digitalRead(I_JKIZQ) == HIGH)
        {
          delay(1000); //Tiempo para evitar falsas lecturas.
          moveLeft();
          while(digitalRead(I_JKIZQ) == HIGH && digitalRead(I_FCX1) == LOW) {  };
          stopMotorX();
        }
      else if(digitalRead(I_JKDER) == HIGH)
        {
          delay(1000);
          moveRight();
          while(digitalRead(I_JKDER) == HIGH && digitalRead(I_FCX2) == LOW) {  };
          stopMotorX();
        }
      else if(digitalRead(I_JKARR) == HIGH)
        {
          delay(1000);
          moveUp();
          while(digitalRead(I_JKARR) == HIGH && digitalRead(I_FCY1) == LOW) {  };
          stopMotorY();
        }
      else if(digitalRead(I_JKABA) == HIGH)
        {
          delay(1000);
          moveDown();
          while(digitalRead(I_JKABA) == HIGH && digitalRead(I_FCY2) == LOW) {  };
          stopMotorY();
        }
      
      if(Serial.available() > 0)
        controller = Serial.read();
      
    }
  Serial.println("Salida del modo Jostick");
  
}

#endif
