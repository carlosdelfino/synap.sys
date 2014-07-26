#include "parameters.h"

void setup() {
  Serial.begin(9660);
  if (DEBUG) {
    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("/DEBUG/ENABLED");
  }
  for (byte b = 0; b < NUM_PINS; b++) {
    pinMode(PINS[b], INPUT);
  }

  if (DEBUG) {
    Serial.print("/DEBUG/NUM_PINS/");
    Serial.println(NUM_PINS);
    Serial.print("/DEBUG/NUM_POTS/");
    Serial.println(NUM_POTS);
  }
}
void loop() {

  byte pares[NUM_PARES][2];
  byte c = 0;
  for (byte b = 0; b < NUM_PARES; b++) {
    pares[b][0] = 255;
    pares[b][1] = 255;
  }

  for (byte p_out = 0; p_out < NUM_PINS && c < NUM_PARES; p_out++) {
    bool nextOutput = false;
    pinMode(PINS[p_out], OUTPUT);
    digitalWrite(PINS[p_out], HIGH);
    if (DEBUG) {
      Serial.print("/DEBUG/PIN/OUTPUT/");
      Serial.println(p_out);
    }

    for (byte p_in = 0; p_in < NUM_PINS; p_in++) {
      if (p_in == p_out) continue;
      bool nextInput = false;
      if (DEBUG) {
        Serial.print("/DEBUG/PIN/INPUT/");
        Serial.println(p_in);
      }

      for (byte par = 0; par < NUM_PARES  && c < NUM_PARES; par++) {
        if (pares[par][0] == p_in || pares[par][1] == p_in) {
          nextInput = true;
          break;
        }
      }
      if (nextInput) continue;

      if (digitalRead(PINS[p_in])) {
        if (DEBUG) {
          Serial.print("/DEBUG/PIN/ON/");
          Serial.println(p_in);
        }
        pares[c][0] = p_in;
        pares[c++][1] = p_out;
        nextOutput = true; // vou para a proxima entrada, apenas um par por output;
        break;
      }
    }
    if (nextOutput) continue;
  }
}
