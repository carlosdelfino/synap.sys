#ifndef PARAMETERS_H
#define PARAMETERS_H 1

#include "Arduino.h"

// macros para ativacao de depuracao
#define D_L_INIT    B00000001
#define D_L_OUTPUT  B00000100
#define D_L_PAIR1   B00001000
#define D_L_PAIR2   B00010000
#define D_L_INPUT   B00100000
//#define DEBUG         ( D_L_INIT | D_L_OUTPUT | D_L_PAIR | D_L_INPUT)
//#define DEBUG         ( D_L_OUTPUT | D_L_PAIR1 | D_L_PAIR2 )
//#define DEBUG         ( D_L_OUTPUT | D_L_PAIR  )
#define DEBUG         (0)
#define DEBUG_INIT    (D_L_INIT   == (DEBUG & D_L_INIT ))
#define DEBUG_OUTPUT  (D_L_OUTPUT == (DEBUG & D_L_OUTPUT))
#define DEBUG_PAIR1   (D_L_PAIR1  == (DEBUG & D_L_PAIR1))
#define DEBUG_PAIR2   (D_L_PAIR2  == (DEBUG & D_L_PAIR2))
#define DEBUG_PAIR    (DEBUG_PAIR1 || DEBUG_PAIR2)
#define DEBUG_INPUT   (D_L_INPUT  == (DEBUG & D_L_INPUT))


// identificacao dos pinos usados,
// o mapeamento nao e direto o pino 1 nao e necessariamente o mesmo pino no arduino
#if DEBUG > 0
const byte PINS[]    = {2, 3, 4, 5, 6, 7};
const byte POTS[]    = {A1, A2, A3, A4};
#elif defined(ARDUINO_AVR_MEGA2560)
const byte PINS[]    = {2, 3, 4, 5, 6, 7, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 37};
const byte POTS[]    = {A1, A2, A3, A4};
#elif defined(ARDUINO_AVR_UNO)
const byte PINS[]    = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const byte POTS[]    = {A1, A2, A3, A4};
#else
#error "Este codigo ainda nao esta pronto para outras plataformas"
#endif



#define DELAY_OUTPUT (DEBUG?100:0)
#define DELAY_INPUT (DEBUG?100:0)
#define DELAY_LOOP (DEBUG?1300:30)
#define DELAY_SHOW_TIME (DEBUG?300:1000)

#endif




