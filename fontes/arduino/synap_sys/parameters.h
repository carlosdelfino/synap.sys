#include "Arduino.h"

#define DEBUG (true)

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
const byte PINS[]    = {2, 3, 4, 5, 6, 7, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 37};
const byte POTS[]    = {A1, A2, A3, A4};
#else
const byte PINS[]    = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const byte POTS[]    = {A1, A2, A3, A4};
#endif

const byte NUM_PINS  = sizeof(PINS) / sizeof(*PINS);
const byte NUM_POTS  = sizeof(POTS) / sizeof(*POTS);
const byte NUM_PARES = NUM_PINS/2;



