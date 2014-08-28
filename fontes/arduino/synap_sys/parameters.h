#ifndef PARAMETERS_H
#define PARAMETERS_H 1

#include "Arduino.h"
#include "debug.h"
#include "pots_lag_types.h"

//#ifdef ARDUINO_ARCH_AVR
//#error "Este codigo foi criado para a plataforma AVR de MCU"
//#endif


// identificacao dos pinos usados,
// o mapeamento nao e direto o pino 1 nao e necessariamente o mesmo pino no arduino
#if DEBUG > 0
#define SYNAP_SYS_BOARD "Arduino DEBUG" 
const byte PINS[]    = {2, 3, 4, 5, 6, 7};
const byte POTS[]    = {A2, A3};
const byte LEDS[]    = {A5};
#elif defined(__AVR_ATmega2560__)  || defined(__AVR_ATmega1280__)  // Arduino Mega e Variantes
//#elif defined(ARDUINO_ARCH_AVR) && defined(ARDUINO_AVR_MEGA2560)
#define SYNAP_SYS_BOARD "Arduino MEGA"
const byte PINS[]    = {24, 30, 4, 5, 7, 22, 28, 23, 29, 33, 27, 35, 2, 3, 6, 37, 26, 32, 31, 25, 34};
const byte POTS[]    = {A1, A2, A3, A4};
const byte LEDS[]    = {36, 38, 39, 40};
#elif defined(__AVR_ATmega328P__) // Arduino UNO
//#elif defined(ARDUINO_ARCH_AVR) && defined(ARDUINO_AVR_UNO)
#define  SYNAP_SYS_BOARD "Arduino UNO"
const byte PINS[]    = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const byte POTS[]    = {A1, A2, A3, A4};
const byte LEDS[]    = {A0, A5};
#else
#error "Este codigo ainda nao esta pronto para outras plataformas"
#endif

// define o lag entre valores para informar um alteracao no valor da leitura analogica
#define POT_LAG (15)
#define POT_LAG_MIN 3
#define POT_LAG_MAX 3

#define MSG_PIN_ON  "1"
#define MSG_PIN_OFF "0"

// indica que deve ser usado LED13 no lugar do time
#if DEBUG > 0
#define USE_LED13_NO_TIME false
#else
#define USE_LED13_NO_TIME true
#endif

#ifndef USE_LED13_NO_TIME
#define USE_LED13_NO_TIME false
#endif

#define DELAY_OUTPUT (DEBUG?10:0)
#define DELAY_INPUT (DEBUG?10:0)
#define DELAY_LOOP (DEBUG?10:3)
#define DELAY_LOOP_PINS (DEBUG?30:30)
#define DELAY_LOOP_POTS (DEBUG?30:10)
#define DELAY_SERIALCMD_READ (DEBUG?2:10)

#if USE_LED13_NO_TIME
#define DELAY_SHOW_TIME (DEBUG?300:500)
#else
#define DELAY_SHOW_TIME (DEBUG?300:3000)
#endif

#endif




