#include "parameters.h"
#include "serialcmd.h"

static inline byte ledEvent(LedCmd ledcmd);

LedCmd ledcmd;
void serialEvent() {
  byte cmd = 0;
  while (Serial.available()) {
    char c = (char)Serial.read();

    if (DEBUG_SERIALCMD)Serial.print(c);

    if (!cmd && c == '/') {
      if (DEBUG_SERIALCMD) {
        Serial.println("/DEBUG//INICIO COMANDO!");
      }

      cmd = CMD_OK;
      continue;
    } else if (cmd == CMD_OK && (c == 'L' || c == 'l')) {
      if (Serial.available() >= 2) {
        c = (char)Serial.read();
        c = (char)Serial.read();
      } else break;
      cmd = CMD_LED;
      continue;
    } else if (cmd == CMD_LED_EVENT) {
      byte res = ledEvent(ledcmd);
      String str = "/LED/CMD/";
      if (res) {
        str += "OK/";
        str += ledcmd->event ? "ON" : "OFF";
        str += "/";
        str += ledcmd->led;
      } else {
        str += "ERROR/";
        str += ledcmd->event ? "ON" : "OFF";
        str += "/";
        str += ledcmd->led;
        str += "/ERROR/";
      }
      Serial.println(str);
    } else if ((cmd >= CMD_LED || cmd <= CMD_LED_FIM)) {
      if(DEBUG_SERIALCMD){
        String str = "/DEBUG/LED/CMD/call_serialEventLedCmd/";
        str += cmd;
        str += "/";
        str += c;
        Serial.println(str);
      }
      serialEventLedCmd(ledcmd, c, &cmd);
      if (DEBUG_SERIALCMD) {
        String str = "/DEBUG/LED/CMD/return_serialEventLedCmd/";
    str += ledcmd->event;
    str += "/";
    str += ledcmd->led;
    str += "/";
    str += LEDS[ledcmd->led];
    str += "/";
    str += cmd;
        Serial.println(str);
      }
    } else if (c == '\n') {
      if (DEBUG_SERIALCMD) {
        Serial.println("/DEBUG/FIM DE COMANDO!");
      }
      cmd = 0;
      break;
    }
  }
  cmd = 0;
}

static inline byte ledEvent(LedCmd ledcmd) {
  bool res = ledcmd->led >= 0 && ledcmd->led < NUM_LEDS;
  if (DEBUG_SERIALCMD) {
    String str = "/DEBUG/LED/exec/event/";
    str += ledcmd->event;
    str += "/";
    str += ledcmd->led;
    str += "/";
    str += LEDS[ledcmd->led];
    Serial.println(str);
  }

  if (res)
    digitalWrite(LEDS[ledcmd->led], ledcmd->event);
  else if (DEBUG_SERIALCMD) {
    String str = "/DEBUG/LED/exec/event/ERROR/";
    str += ledcmd->event;
    str += "/";
    str += ledcmd->led;
    str += "/";
    str += LEDS[ledcmd->led];
    str += "/ERROR";
    Serial.println(str);
  }

  return res;
}

void serialEventLedCmd(LedCmd ledcmd, char c, byte *cmd_p) {
  byte cmd = (*cmd_p);
  (*cmd_p) = 0;

  if (DEBUG_SERIALCMD) {
    String str = "/DEBUG/LED/serialEventLedCmd/";
    str += ledcmd->led;
    str += "/";
    str += ledcmd->event;
    str += "/";
    str += cmd;
    str += "/";
    str += c;
    
    Serial.println(str);
  }

  if (cmd == CMD_LED && c == '/') {
    byte  num = Serial.parseInt();
    ledcmd->led = num;
    if (DEBUG_SERIALCMD) {
      String str = "/DEBUG/LED/num/";
      str += num;
      str += "/";
      Serial.println(str);
    }
    (*cmd_p) =  CMD_LED_NUM_OK;

  } else if (cmd == CMD_LED_NUM_OK && c == '/') {
    if (Serial.available() >= 2) {
      c = (char)Serial.read();
      c = (char)Serial.read();
    } else return;

    ledcmd->event = (c == 'N' || c == 'n');
    (*cmd_p) = CMD_LED_EVENT;
  }
}

void setupLeds() {
  ledcmd = (LedCmd)malloc(sizeof(LedCmd));

  Serial.println("\n/init/leds/check");
  String str;
  for (byte b = 0; b < NUM_LEDS; b++) {

    ledcmd->led = b;

    pinMode(LEDS[ledcmd->led], OUTPUT);

    ledcmd->event = 1;
    ledEvent(ledcmd);

    delay(20);

    ledcmd->event = !ledcmd->event;
    ledEvent(ledcmd);

    if (DEBUG_INIT) {
      str = "/DEBUG/LED/";
      str += b;
      Serial.println(str);
    }
  }
  ledcmd->led = 99;
  ledcmd->event = 0;
  Serial.println("\n/init/leds/ok");
}
