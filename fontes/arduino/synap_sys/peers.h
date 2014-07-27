#ifndef PEERS_H
#define PEERS_H

#include "parameters.h"

const byte NUM_PINS  = sizeof(PINS) / sizeof(*PINS); // para saber o tamanho de um array
const byte NUM_PEERS = NUM_PINS / 2;

void movePeers();

bool pairFree(byte pair);
bool pairUsed(byte pair);

byte pairPin1(byte);
byte pairPin2(byte);

void setOldPair(byte pair, byte pin_1, byte pin_2);
void setPair(byte pair, byte pin_1, byte pin_2);

bool removeOldPair(byte);
bool isOldPair(byte, bool r = true);

void resetOldPeers();
void resetPeers();

void showPeers();

#define PIN_1 0
#define PIN_2 1
byte oldPairPin1(byte);
byte oldPairPin2(byte);

bool usedPin(byte pair, byte pin);

#endif
