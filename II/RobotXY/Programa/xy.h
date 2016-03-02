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

//Comprobaci—n funcionamiento

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

//Comprobaci—n funcionamiento

#define I_RUNY  34

//Freno
#define O_FRENO 43 //Normalmente cerrado

//Piloto estado eje

#define O_PAY   45

/**********************************************/
//
//      FUNCIONES DE INTERRUPCION
//
/**********************************************/

//Interrupciones de los pines
#define INT_EMERG 	18
#define INT_ENX     2
#define INT_ENY     3
#define INT_INTFINC 19

/**********************************************/
//
//            ESTADOS DEL SISTEMA
//
/**********************************************/



#endif