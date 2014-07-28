#ifndef PARAMETERS_H
#define PARAMETERS_H 1

#include "Arduino.h"
#include "pots_lag_types.h"

//#ifdef ARDUINO_ARCH_AVR
//#error "Este codigo foi criado para a plataforma AVR de MCU"
//#endif

// macros para ativacao de depuracao
#define D_L_INIT    B00000001
#define D_L_OUTPUT  B00000100
#define D_L_PAIR1   B00001000
#define D_L_PAIR2   B00010000
#define D_L_INPUT   B00100000
#define D_L_POTS    B10000000

//#define DEBUG         ( D_L_INIT | D_L_OUTPUT | D_L_PAIR | D_L_INPUT)
//#define DEBUG         ( D_L_OUTPUT | D_L_PAIR1 | D_L_PAIR2 )
//#define DEBUG         ( D_L_OUTPUT | D_L_PAIR  )
//#define DEBUG          (D_L_POTS)

#ifndef DEBUG
#define DEBUG         (0)
#endif

#define DEBUG_INIT    (D_L_INIT   == (DEBUG & D_L_INIT ))
#define DEBUG_OUTPUT  (D_L_OUTPUT == (DEBUG & D_L_OUTPUT))
#define DEBUG_PAIR1   (D_L_PAIR1  == (DEBUG & D_L_PAIR1))
#define DEBUG_PAIR2   (D_L_PAIR2  == (DEBUG & D_L_PAIR2))
#define DEBUG_PAIR    (DEBUG_PAIR1 || DEBUG_PAIR2)
#define DEBUG_INPUT   (D_L_INPUT  == (DEBUG & D_L_INPUT))
#define DEBUG_POTS    (D_L_POTS   == (DEBUG & D_L_POTS))

// identificacao dos pinos usados,
// o mapeamento nao e direto o pino 1 nao e necessariamente o mesmo pino no arduino
#if DEBUG > 0
const byte PINS[]    = {2, 3, 4, 5, 6, 7};
const byte POTS[]    = {A2, A3};
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
//#elif defined(ARDUINO_ARCH_AVR) && defined(ARDUINO_AVR_MEGA2560)
const byte PINS[]    = {2, 3, 4, 5, 6, 7, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 37};
const byte POTS[]    = {A1, A2, A3, A4};
#elif defined(__AVR_ATmega328__) || defined(__AVR_ATmega128__)
//#elif defined(ARDUINO_ARCH_AVR) && defined(ARDUINO_AVR_UNO)
const byte PINS[]    = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const byte POTS[]    = {A1, A2, A3, A4};
#else
#error "Este codigo ainda nao esta pronto para outras plataformas"
#endif

// define o lag entre valores para informar um alteracao no valor da leitura analogica
#define POT_LAG (15)
#define POT_LAG_MIN 3
#define POT_LAG_MAX 3

// indica que deve ser usado LED13 no lugar do time
#define USE_LED13_NO_TIME true


#ifndef USE_LED13_NO_TIME
#define USE_LED13_NO_TIME false
#endif

#define DELAY_OUTPUT (DEBUG?10:0)
#define DELAY_INPUT (DEBUG?10:0)
#define DELAY_LOOP (DEBUG?10:3)
#define DELAY_LOOP_PINS (DEBUG?30:30)
#define DELAY_LOOP_POTS (DEBUG?30:10)

#if USE_LED13_NO_TIME
#define DELAY_SHOW_TIME (DEBUG?300:500)
#elif
#define DELAY_SHOW_TIME (DEBUG?300:3000)
#endif

#endif




