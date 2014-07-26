#include "Arduino.h"

// macros para ativacao de depuracao
#define D_L_INIT   B00000001
#define D_L_OUTPUT B00001000
#define D_L_PAR    B00010000
#define D_L_INPUT  B00100000
//#define DEBUG         ( D_L_INIT | D_L_OUTPUT | D_L_PAR | D_L_INPUT)
//#define DEBUG         ( D_L_OUTPUT | D_L_PAR )
#define DEBUG         (0)
#define DEBUG_INIT    (D_L_INIT   == (DEBUG & D_L_INIT ))
#define DEBUG_OUTPUT  (D_L_OUTPUT == (DEBUG & D_L_OUTPUT))
#define DEBUG_PAIR    (D_L_PAIR    == (DEBUG & D_L_PAIR))
#define DEBUG_INPUT   (D_L_INPUT  == (DEBUG & D_L_INPUT))


// identificacao dos pinos usados,
// o mapeamento nao e direto o pino 1 nao e necessariamente o mesmo pino no arduino
#if DEBUG > 0
const byte PINS[]    = {2, 3, 4, 5, 6, 7};
const byte POTS[]    = {A1, A2, A3, A4};
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
const byte PINS[]    = {2, 3, 4, 5, 6, 7, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 37};
const byte POTS[]    = {A1, A2, A3, A4};
#else
const byte PINS[]    = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const byte POTS[]    = {A1, A2, A3, A4};
#endif

const byte NUM_PINS  = sizeof(PINS) / sizeof(*PINS); // para saber o tamanho de um array
const byte NUM_POTS  = sizeof(POTS) / sizeof(*POTS);
const byte NUM_PEERS = NUM_PINS / 2;

// DEFINICAO DO ARRAY QUE GUARDA PEERS CONECTADOS
#define PIN_1 0
#define PIN_2 1
byte _peers[NUM_PEERS][2];
#define pairFree(pair) (_peers[pair][0] == 255 || _peers[pair][1] == 255)
#define pairUsed(pair) (!pairFree(pair))
#define pairPin1(pair) (_peers[pair][0])
#define pairPin2(pair) (_peers[pair][1])
#define peers(pair,pin_1,pin_2) do{\
    byte p = pair;\
    _peers[p][PIN_1] = pin_1;\
    _peers[p][PIN_2] = pin_2;\
  }while(false);
#define usedPin(pair,pin)  (_peers[pair][PIN_1] == pin || _peers[pair][PIN_2] == pin)




#define DELAY_OUTPUT (DEBUG?100:0)
#define DELAY_INPUT (DEBUG?100:0)
#define DELAY_LOOP (DEBUG?500:30)




