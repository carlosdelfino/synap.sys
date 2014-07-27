#include "Arduino.h"
#include "peers.h"

static byte _peers[NUM_PEERS][2];
static byte _oldPeers[NUM_PEERS][2];

bool pairFree(byte pair) {
  return (_peers[pair][0] == 255 || _peers[pair][1] == 255);
}

bool pairUsed(byte pair) {
  return (_peers[pair][0] != 255 && _peers[pair][1] != 255);
}

byte pairPin1(byte pair) {
  return  (_peers[pair][0]);
}
byte pairPin2(byte pair) {
  return (_peers[pair][1]);
}

byte oldPairPin1(byte pair) {
  return (_oldPeers[pair][0]);
}
byte oldPairPin2(byte pair) {
  return (_oldPeers[pair][1]);
}

bool usedPin(byte pair, byte pin) {
  return  (_peers[pair][PIN_1] == pin || _peers[pair][PIN_2] == pin);
}



void movePeers() {
  /* parece que memmove esta parametrizado errado
   *
  int SIZE_OLDPEERS = ((sizeof(_oldPeers[0]) / sizeof(*_oldPeers[0]))
                       * (sizeof(_oldPeers) / sizeof(*_oldPeers)));
  memmove(_oldPeers, _peers, SIZE_OLDPEERS);
  *
  */
  for (byte b = 0; b < NUM_PEERS; b++) {
    _oldPeers[b][0] = _peers[b][0];
    _oldPeers[b][1] = _peers[b][1];
    _peers[b][0] = 255;
    _peers[b][1] = 255;
  }
}

void setOldPair(byte pair, byte pin_1, byte pin_2) {
  if (DEBUG_PAIR1) {
    String str1 = "/DEBUG/PAIR/SETOLDPAIR/";
    str1 += pair;
    str1 += "/";
    str1 += pin_1;
    str1 += "/";
    str1 += pin_2;
    Serial.println(str1);
  }
  _oldPeers[pair][PIN_1] = pin_1;
  _oldPeers[pair][PIN_2] = pin_2;
}

void setPair(byte pair, byte pin_1, byte pin_2) {
  if (DEBUG_PAIR1) {
    String str1 = "/DEBUG/PAIR/SETPAIR/";
    str1 += pair;
    str1 += "/";
    str1 += pin_1;
    str1 += "/";
    str1 += pin_2;
    Serial.println(str1);
  }
  _peers[pair][PIN_1] = pin_1;
  _peers[pair][PIN_2] = pin_2;
}

void resetOldPeers() {
  if (DEBUG_PAIR1) {
    String str1 = "/DEBUG/PAIR/RESETOLDPEERS/";
    Serial.println(str1);
  }

  for (byte b = 0; b < NUM_PEERS; b++) {
    setOldPair(b, 255, 255);
  }
}

void resetPeers() {
  if (DEBUG_PAIR1) {
    String str1 = "/DEBUG/PAIR/RESETPEERS/";
    Serial.println(str1);
  }

  for (byte b = 0; b < NUM_PEERS; b++) {
    setPair(b, 255, 255);
  }
}

bool removeOldPair(byte p) {
  if (DEBUG_PAIR1) {
    String str1 = "/DEBUG/PAIR/REMOVEOLD/";
    str1 += p;
    Serial.println(str1);
  }

  return isOldPair(p, true);
}

bool isOldPair(byte p, bool r) {
  String str1;

  if (DEBUG_PAIR1) {
    str1 = "/DEBUG/PAIR/ISOLD/BEGIN/";
    str1 += p;
    if (r) str1 += "/REMOVE/";
    Serial.println(str1);
  }
  bool isOld = false;

  if (pairFree(p)) {
    if (DEBUG_PAIR1) {
      str1 = "/DEBUG/PAIR/ISOLD/";
      str1 += p;
      if (r) str1 += "/NOTINUSE/";
      Serial.println(str1);
    }
  } else for (byte b = 0; b < NUM_PEERS; b++) {
      if (DEBUG_PAIR1) {
        str1 = "/DEBUG/PAIR/ISOLD/";
        str1 += p;
        str1 += "/";
        str1 += _peers[p][PIN_1];
        str1 += "/";
        str1 += _peers[p][PIN_2];
        str1 += "/";
        str1 += "/";
        str1 += b;
        str1 += "/";
        str1 += _oldPeers[b][PIN_1];
        str1 += "/";
        str1 += _oldPeers[b][PIN_2];
        str1 += "/";
        Serial.println(str1);
      }

      if ((_oldPeers[b][PIN_1] == _peers[p][PIN_1] && _oldPeers[b][PIN_2] == _peers[p][PIN_2])
          || (_oldPeers[b][PIN_1] == _peers[p][PIN_2] && _oldPeers[b][PIN_2] == _peers[p][PIN_1])) {

        if (DEBUG_PAIR1) {
          str1 = "/DEBUG/PAIR/ISOLD/";
          str1 += p;
          str1 += "/";
          str1 +=  _peers[p][PIN_1];
          str1 += "/";
          str1 +=  _peers[p][PIN_2];
          Serial.println(str1);
        }

        if (r) {
          _oldPeers[b][PIN_1] = _oldPeers[b][PIN_2] = 255;
          if (DEBUG_PAIR1) {
            str1 = "/DEBUG/PAIR/ISOLD/REMOVED/";
            str1 += p;
            Serial.println(str1);
          }
        }

        isOld = true;
        break;

      } else if (DEBUG_PAIR1) {
        str1 = "/DEBUG/PAIR/ISOLD/";
        str1 += p;
        if (r) str1 += "/NEW/";
        Serial.println(str1);
      }

    } // FINALIZA ELSE FOR
  if (DEBUG_PAIR1) {
    str1 = "/DEBUG/PAIR/ISOLD/END/";
    str1 += p;
    if (r) str1 += "/REMOVE/";
    Serial.println(str1);
  }
  return isOld;

}

void showPeers() {

  String str1;
  if (DEBUG_PAIR1) {
    str1 = "/DEBUG/PAIR/SHOWPEERS/BEGIN";
    Serial.println(str1);
    str1 = "/DEBUG/PAIR/SHOWPEERS/SHOWON";
    Serial.println(str1);
  }

  for (byte b = 0; b < NUM_PEERS; ++b) {
    // o pair esta em uso, e nao existe anteriormente
    if (pairUsed(b))
      if (!removeOldPair(b)) {
        str1 = "/PIN/";
        str1 += pairPin1(b);
        str1 += "/";
        str1 += pairPin2(b);

        str1 += "/ON";
        Serial.println(str1);
        Serial.flush();
      }
  }// finaliza if if

  if (DEBUG_PAIR1) {
    str1 = "/DEBUG/PAIR/SHOWPEERS/SHOWOFF";
    Serial.println(str1);
  }
  for (byte b = 0; b < NUM_PEERS; b++) {
    byte pin1 = oldPairPin1(b);
    byte pin2 = oldPairPin2(b);
    if (pin1 != pin2 && pin2 != 255) {
      str1 = "/PIN/";
      str1 += pin1;
      str1 += "/";
      str1 += pin2;
      str1 += "/off";
      Serial.println(str1);
      Serial.flush();
    }

  }
  if (DEBUG_PAIR1) {
    str1 = "/DEBUG/PAIR/SHOWPEERS/END";
    Serial.println(str1);
  }

}
