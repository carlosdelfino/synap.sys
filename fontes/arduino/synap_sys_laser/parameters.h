#ifndef PARAMETERS_H
#define PARAMETERS_H 1

#include "Arduino.h"
#include "limits.h"
#include "debug.h" 


#if DEBUG > 0
#define SYNAP_SYS_BOARD "Arduino DEBUG" 
const byte LASERS[]    = { 2,  3,  4,  5,  6};
const byte SERVOS[]    = {12, 11, 10,  9,  8};
#elif defined(__AVR_ATmega2560__)  || defined(__AVR_ATmega1280__)  // Arduino Mega e Variantes
//#elif defined(ARDUINO_ARCH_AVR) && defined(ARDUINO_AVR_MEGA2560)
#define SYNAP_SYS_BOARD "Arduino MEGA"
const byte LASERS[]    = { 2,  3,  4,  5,  6};
const byte SERVOS[]    = {12, 11, 10,  9,  8};
#elif defined(__AVR_ATmega328P__) // Arduino UNO
//#elif defined(ARDUINO_ARCH_AVR) && defined(ARDUINO_AVR_UNO)
#define  SYNAP_SYS_BOARD "Arduino UNO"
const byte LASERS[]    = { 2,  3,  4,  5,  6};
const byte SERVOS[]    = {12, 11, 10,  9,  8};
#else
#error "Este codigo ainda nao esta pronto para outras plataformas"
#endif


const byte NUM_LASERS  = sizeof(LASERS) / sizeof(*LASERS);
const byte NUM_SERVOS  = sizeof(SERVOS) / sizeof(*SERVOS);

#define MAX_SERVO_ANGLE 179
#define MIDLE_SERVO_ANGLE (MAX_SERVO_ANGLE/2)
#define MIN_SERVO_ANGLE 0
#define SERVO_DELAY_TEST 250
#endif
