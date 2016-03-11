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

bool useJoystick = true;
bool freno = false;
int tWaitLedAlarm = 100;
int tWaitJoystick = 100;
int tWait = 3000;
int tWaitFC = 2000;
int maxVelocityPoint = 750;
int midVelocityPoint = 250;
void intEncoderX();
void intEncoderY();
void interruptEmergency();
void intActivateEndstop();
void motorHome(unsigned short motorSelect); // 0 -> X, 1 -> Y
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
void calibrate(unsigned short motor); // Steps per mm.
void moveXmm(int mm); // + -> Right, - -> Left
void moveYmm(int mm); // + -> Up, - -> Down
unsigned short getStateEndStops();
void adaptVelocityX(int remainingSteps);
void adaptVelocityY(int remainingSteps);
void selectVelocityX(unsigned short velocity);
void selectVelocityY(unsigned short velocity);
void writePosition(unsigned short axis);

void writePosition(unsigned short axis)
{
  if(axis == 0)
    Serial.print(contadorPasosX/(int)stepsX);
  else
    Serial.print(contadorPasosY/(int)stepsY);
  Serial.print("\n");
}

void adaptVelocityX(int remainingSteps)
{
  if(remainingSteps > maxVelocityPoint)
    selectVelocityX(3);
  else if(remainingSteps > midVelocityPoint)
    selectVelocityX(2);
  else
    selectVelocityX(1);
}

void adaptVelocityY(int remainingSteps)
{
  if(remainingSteps > maxVelocityPoint)
    selectVelocityY(3);
  else if(remainingSteps > midVelocityPoint)
    selectVelocityY(2);
  else
    selectVelocityY(1);
}

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

//TODO CAMBIAR UP POR DOWN
void moveDown()
{
  deactiveBrake();
  digitalWrite(O_STRY,LOW);
  digitalWrite(O_STFY,HIGH);
  digitalWrite(O_PAY,HIGH);
  dirMotorY = true;
}

void moveUp()
{
  deactiveBrake();
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
      int stepsInicial = contadorPasosX;
      int contadorTarget = contadorPasosX + stepsX * mm;
      Serial.println("HOLA");
      moveRight();
      
      while(contadorTarget > contadorPasosX && digitalRead(I_FCX2) == LOW) 
      {
	      adaptVelocityX(contadorTarget-contadorPasosX);
      }
      stopMotorX();
    }
  else if (mm < 0)
    {
      int contadorTarget = contadorPasosX + stepsX * mm;
      moveLeft();
      while(contadorTarget < contadorPasosX && digitalRead(I_FCX1) == LOW) 
      {
        adaptVelocityX(contadorPasosX - contadorTarget); 
      };
      stopMotorX();
    }
  else
    Serial.println("Move 0 mm");
}

void moveYmm(int mm)
{
  stopMotorY();
  stopMotorX();
  int contadorTarget = contadorPasosY + stepsY * mm;
  if(mm > 0)
    {
      //Serial.println("Ymm");
      moveUp();
      //Serial.println("moveUp");
      //Serial.println(contadorTarget);
      
      while(contadorTarget > contadorPasosY && digitalRead(I_FCY1) == LOW) 
      { 
        //Serial.println(contadorPasosY);
	adaptVelocityY(contadorTarget - contadorPasosY);
      }
      stopMotorY();
    }
  else if (mm < 0)
    {
      //Serial.println("1000");
      
      moveDown();
      //Serial.println("1002");
      while(contadorTarget < contadorPasosY && digitalRead(I_FCY2) == LOW) 
      {
  	adaptVelocityY(contadorPasosY - contadorTarget);
      }
      //Serial.println("1001");
      stopMotorY();
    }
  else
    Serial.println("Move 0 mm");
}


void moveXSteps(int steps)
{
  int stepsTarget = contadorPasosX + steps;
  //Serial.println("Move Steps");
  if (steps > 0)
  {
     moveRight();
     //Serial.println("Derecha");
     //Serial.println(stepsTarget);
     while(stepsTarget > contadorPasosX && digitalRead(I_FCX2) == LOW) 
    {
      //Serial.println(contadorPasosX);
 	adaptVelocityX(stepsTarget-contadorPasosX);
    };
     stopMotorX();
  }
  else 
  {
    //Serial.println("Izquierda");
     moveLeft();
      //Serial.println(stepsTarget);
     while(stepsTarget < contadorPasosX && digitalRead(I_FCX1) == LOW) 
     {
      //Serial.println(contadorPasosX);
	adaptVelocityX(stepsTarget-contadorPasosX);
     };
     //Serial.println("Salgo");
     stopMotorX();
  }

}

void moveYSteps(int steps) {

  int stepsTarget = contadorPasosY + steps;

  if (steps > 0)
  {
     moveUp();
     while(stepsTarget > contadorPasosY && digitalRead(I_FCY1) == LOW) 
     {
	adaptVelocityY(stepsTarget - contadorPasosY);
     };
     stopMotorY();
  }
  else 
  {
     moveDown();
     while(stepsTarget < contadorPasosY && digitalRead(I_FCY2) == LOW) 
     {
  	adaptVelocityY(contadorPasosY - stepsTarget);
     };
     stopMotorY();
  }
}

void checkAlarm()
{
  if(I_ALARMAX || I_ALARMAY)
  {
    stopMotorY();
    stopMotorX();
    activeBrake();
    bool led = HIGH;
    while(I_ALARMAX || I_ALARMAY) 
    {
      digitalWrite(O_PAX,led);
      digitalWrite(O_PAY,led);
      delay(tWaitLedAlarm);
      led = !led;
     };
  }
}



void stopMotorX()
{
  digitalWrite(O_STFX,LOW);
  digitalWrite(O_STRX,LOW);
  digitalWrite(O_PAX,LOW);
}

void stopMotorY()
{
  digitalWrite(O_STFY,LOW);
  digitalWrite(O_STRY,LOW);
  digitalWrite(O_PAY,LOW);
  activeBrake();
}

void activeBrake()
{

  digitalWrite(O_FRENO, FRENOACT);
  freno = true;
  delay(tWait);  
}

void deactiveBrake()
{
  digitalWrite(O_FRENO, FRENODES);
  freno = false;
  //delay(tWait);
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
  pinMode(O_STRX,OUTPUT);
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
  attachInterrupt(4,intActivateEndstop,FALLING);
  attachInterrupt(5,interruptEmergency,FALLING);
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
  Serial.print(digitalRead(I_CMARCHA));
 // if(digitalRead(I_CMARCHA) == HIGH)
   // {
      if(motorSelect == 0)
        {//Motor X
          //Serial.println("Homing X");
          selectVelocityX(1);
          stopMotorY();
          moveLeft();
          //delay(500);
          //delay(tWaitFC);
          while(!digitalRead(I_FCX1)){ };
          
          contadorPasosX = 0;
          moveRight();
	  
          //while(digitalRead(I_FCX1)) {};
          delay(tWaitFC);
          stopMotorX();
        }
      else if(motorSelect == 1)
        {// Motor Y
          //Serial.println("Homing Y");
          selectVelocityY(1);
          stopMotorX();
          deactiveBrake();
          delay(50);
          moveDown();
         //delay(tWaitFC);
          while(!digitalRead(I_FCY2)) { };
          
          contadorPasosY = 0;
          moveUp();
//          while(digitalRead(I_FCY2)){ };
  	  delay(tWaitFC);        
          stopMotorY();
        }
      else
        {//UNKNOWN Motor
          //Serial.println("ERROR: motorSelect > 1");
          interruptEmergency();
        }
    
  //else
    //Serial.println("ERROR: The motorSelect is not active");
}

// Steps expresed in mm.
void calibrate(unsigned short motor)
{
  //Start [0,0]
  if(motor == 0)
    {
      selectVelocityX(2);
      moveRight();
      while(!digitalRead(I_FCX2)) { };
      
      int stepsXAux = contadorPasosX;
     /* Serial.print("\nSteps motor X per");
      Serial.print(mmX);
      Serial.print(": ");
      Serial.println(stepsXAux);
      *///Serial.println(contador
      moveLeft();
      while(digitalRead(I_FCX2)){};
      stopMotorX();
      
      stepsX = (stepsXAux)/((float)mmX);
      //Serial.printlnln(stepsX);
    }
  else if(motor == 1)
    {
      selectVelocityX(2);
      moveUp();
      while(!digitalRead(I_FCY1)) { };
      
      int stepsYAux = contadorPasosY;
      //Serial.println("\nSteps per");
      //Serial.println(mmY);
      //Serial.println(": ");
      //Serial.printlnln(stepsYAux);
      moveDown();
      while(digitalRead(I_FCY1)){};
      
      stopMotorY();
      
      stepsY = (stepsYAux)/((float) mmY);
      //Serial.printlnln(stepsY);
    }
  else
    Serial.println("ERROR: Calibrate motor");
}

void intEncoderX()
{
  //Serial.println("EncoderX");
  if(dirMotorX == DIRBACK)
    contadorPasosX--;
  else
        contadorPasosX++;
  //Serial.println(contadorPasosX);
}

void intEncoderY()
{ //TODO Cambiar pasos
   //Serial.println("EncoderY");
  if(dirMotorY != DIRBACK)
    contadorPasosY--;
  else
    contadorPasosY++;
     // Serial.println(contadorPasosY);
}

void intActivateEndstop()
{
  //Serial.printlnln("ENDSTOP");
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

void interruptEmergency()
{
  //Serial.printlnln("EMERGENCIA");
  stopMotorX();
  stopMotorY();
  deactiveMotors();
  delayMicroseconds(10000);
  //TODO Comprobar si es LOW o HIGH en el de emergencia.
  while(digitalRead(INT_EMERG) == LOW) { };
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

// TODO contolar maximo y minimo con finales de carrera con joystick
// Inicio de velocidad adaptativo
// Activar motores tras emergencia.
// Cambiar la velocidad por serial
// REdundancia
// Monitorizacion serial
// 
void controlJoystick()
{
  if(useJoystick)
  {
    selectVelocityX(2);
    selectVelocityY(2);
   // Serial.println("JOYSTICK");
      if(digitalRead(I_JKIZQ) == HIGH)
        {
         //  Serial.println("JOYSTICK1");
          delay(tWaitJoystick); //Tiempo para evitar falsas lecturas.
          moveLeft();
//          while(digitalRead(I_JKIZQ) == HIGH && digitalRead(I_FCX1) == LOW) {  };
//          stopMotorX();
        }
      if(digitalRead(I_JKDER) == HIGH)
        {
         //  Serial.println("JOYSTICK2");
          delay(tWaitJoystick);
          moveRight();
//          while(digitalRead(I_JKDER) == HIGH && digitalRead(I_FCX2) == LOW) {  };
//          stopMotorX();
        }
      if(digitalRead(I_JKARR) == HIGH)
        {
           //Serial.println("JOYSTICK3");
          delay(tWaitJoystick);
          moveUp();
//          while(digitalRead(I_JKARR) == HIGH && digitalRead(I_FCY1) == LOW) {  };
//          stopMotorY();
        }
      if(digitalRead(I_JKABA) == HIGH)
        {
           //Serial.println("JOYSTICK4");
          delay(tWaitJoystick);
          moveDown();
//          while(digitalRead(I_JKABA) == HIGH && digitalRead(I_FCY2) == LOW) {  };
//          stopMotorY();
        }
      if(digitalRead(I_JKDER) == LOW && digitalRead(I_JKIZQ) == LOW && digitalRead(I_JKABA) == 
LOW && digitalRead(I_JKARR) == LOW)
        {
	        stopMotorX();
	        stopMotorY();
        }
  }
}
// 4149 X
// 3223 Y

void setPositionX(int value)
{
  contadorPasosX = value;
}


void readSerial()
{
  while (Serial.available() > 0) 
  {
    int action = Serial.parseInt();
    int axis = Serial.parseInt();
    int value = Serial.parseInt();
/*    Serial.print("action:");
    Serial.print(action);
    Serial.print("Axis:");
    Serial.print(axis);
    Serial.print("Value");
    //Serial.println(value);*/
    if (Serial.read() == '\n') {
      if (!useJoystick)
      {
  	switch(action)
	{
	  case 0:
	    if(axis == 0)
	       moveXmm(value);
	    else
	       moveYmm(value);
//	    contadorPasosX = value;

	  break;
 	  case 1:
	    if(axis == 0)
	       moveXmm(value*-1);
	    else
	       moveYmm(value*-1);
	  break;
	  case 2:
	    if(axis == 0)
	       moveXSteps(value);
	    else
	       moveYSteps(value);
	  break;
	  case 3:
	    if(axis == 0)
	       moveXSteps(value*-1);
	    else
	       moveYSteps(value*-1);
	  break;
	  case 4:
	    calibrate(axis);
	  break;
	  case 5:
	    motorHome(axis);
	  break;
	  case 6:
      if (!value)
      {
        stopMotorX();
        stopMotorY();
      }
	    useJoystick = value;
	  break;
	  case 7:
	    writePosition(axis);
	  break;
    case 8:
      if (axis == 0)
      setPositionX(value);
    break;
	  default:
	    Serial.print("He recibido");
	    Serial.println(action);
     break;
	}
      }
	else
	{
	  if(action == 6)
	    useJoystick = value;
    if(action == 7)
      writePosition(axis);
	}
    }
  }


}

#endif
