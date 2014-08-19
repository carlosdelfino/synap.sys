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



#define TEMP_INTERNAL_TOS  320.57
#define TEMP_INTERNAL_K    1.22

#define MEAN_TEMP_ROOM      10
#define MEAN_TEMP_INTERNAL  10
#define MEAN_TEMP_HEAT_SINK  10


#define PIN_TEMP_HEAT_SINK A0
#define PIN_TEMP_ROOM      A1


const byte NUM_LASERS  = sizeof(LASERS) / sizeof(*LASERS);
const byte NUM_SERVOS  = sizeof(SERVOS) / sizeof(*SERVOS);

#define MAX_SERVO_ANGLE 179
#define MIDLE_SERVO_ANGLE (MAX_SERVO_ANGLE/2)
#define MIN_SERVO_ANGLE 0
#define SERVO_DELAY_TEST 250

#define DELAY_TEMP_ROOM          (1000)
#define DELAY_TEMP_HEAT_SINK     (1000)
#define DELAY_TEMP_INTERNAL      (1000)      // delay em milesegundos
#define DELAY_TEMP_INTER_READ    (10 * 1000) // delay em microsegundos
#define DELAY_SETUP_INTER_LASERS (50)

#define DELAY_SERIAL_CMD   (3)
#define TIMEOUT_SERIAL_CMD (6)




// user apenas DEFAULT ou INTERNAL
#define TEMP_AREF  (DEFAULT) // cuidado ao fazer esta mudanca vc pode danificar o arduino
#endif
