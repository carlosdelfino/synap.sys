#include "parameters.h"
#include "serialcmd.h"
#include "limits.h"
#include <Servo.h>

//#line 6

/**
static  void parseSerialLaserCmd(Cmd cmds, char c, byte * cmd_p);
static  void parseSerialServoCmd(Cmd cmds, char c, byte * cmd_p);
static  byte laserCmdEvent(Cmd cmds);
static  byte servoCmdEvent(Cmd cmds);
**/
byte servoCmdEvent(Cmd cmds);
byte laserCmdEvent(Cmd cmds);
byte serialWaitAvailable(byte s = 1, long timeOut = TIMEOUT_SERIAL_CMD);

Servo servosObj[NUM_SERVOS];

Cmd cmds = (Cmd)malloc(sizeof(Cmd));

byte serialWaitAvailable(byte s, long timeOut) {
  long lastread = millis();
  byte a;
  do {
    delay(DELAY_SERIAL_CMD);
    a = Serial.available();
  } while ( (a < s) && ( (millis() - lastread) < timeOut));
  return a;
}

void setupSerialCmd() {
  cmds->event = 0;
  cmds->id = 99;
}

void attachServo(Cmd cmd) {
  servosObj[cmd->id].attach(SERVOS[cmd->id]);
}


byte servoCmdEvent(Cmd cmd) {
  bool res = (cmd->id >= 0 && cmd->id < NUM_SERVOS) || (cmd->id == CODE_ALL_SERVOS);
  res = res && (cmd->event >= MIN_SERVO_ANGLE && cmd->event <= MAX_SERVO_ANGLE);
  if (DEBUG_SERIALCMD) {
    String str = "/DEBUG/SERVO/exec/event/";
    str += cmd->id;
    str += "/";
    str += SERVOS[cmd->id];
    str += "/";
    str += cmd->event;
    str += "/";
    str += res;
    Serial.println(str);
  }

  if (res) {
    if ( cmd->id == CODE_ALL_SERVOS)
      for (byte b = 0; b < NUM_SERVOS; b++) {
        servosObj[b].write(cmd->event);
      }
    else
      servosObj[cmd->id].write(cmd->event);
  }  else if (DEBUG_SERIALCMD) {
    String str = "/DEBUG/SERVO/exec/event/ERROR/";
    str += cmd->event;
    str += "/";
    str += cmd->id;
    str += "/";
    str += LASERS[cmd->id];
    str += "/ERROR";
    Serial.println(str);
  }

  return res;
}

byte laserCmdEvent(Cmd cmds) {
  if (DEBUG_SERIALCMD) {
    String str = "/DEBUG/LASER/exec/event/";
    str += cmds->event;
    str += "/";
    str += cmds->id;
    str += "/";
    str += LASERS[cmds->id];
    Serial.println(str);
  }

  bool status = false;
  if (cmds->id >= 0 && cmds->id < NUM_LASERS) {
    digitalWrite(LASERS[cmds->id], cmds->event);
    status =  true;
  } else if (cmds->id == CODE_ALL_LASERS) {
    for (byte b = 0; b < NUM_LASERS; b++) {
      digitalWrite(LASERS[b], cmds->event);
    }
    status = true;
  } else if (DEBUG_SERIALCMD) {
    String str = "/DEBUG/LASER/exec/event/ERROR/";
    str += cmds->event;
    str += "/";
    str += cmds->id;
    str += "/";
    str += LASERS[cmds->id];
    str += "/ERROR";
    Serial.println(str);
  }

  return status;
}

bool parseSerialLaserCmd() {
  byte  num = Serial.parseInt();
  cmds->id = num;
  if (DEBUG_SERIALCMD) {
    String str = "/DEBUG/LASER/num/";
    str += num;
    str += "/";
    Serial.println(str);
  }
  char c;
  if (serialWaitAvailable(3)) {
    Serial.read();
    Serial.read();
    c = (char)Serial.read();
  } else return false;

  if (DEBUG_SERIALCMD) {
    String str = "/DEBUG/LASER/cmd/";
    str += (c == 'N' || c == 'n') ? "on" : "off";
    str += "/";
    Serial.println(str);
  }
  cmds->event = (c == 'N' || c == 'n');
  byte res = laserCmdEvent(cmds);
  String str = "/LASER/CMD/";
  if (res) {
    str += "OK/";
    str += cmds->id;
    str += "/";
    str += cmds->event ? "ON" : "OFF" ;
    str += "/OK/";
  } else {
    str += "ERROR/";
    str += cmds->id;
    str += "/";
    str += cmds->event;
    str += "/ERROR/";
  }
  Serial.println(str);
  Serial.flush();
  return res;
}

bool parseSerialServoCmd() {
  byte  num = Serial.parseInt();
  cmds->id = num;
  if (DEBUG_SERIALCMD) {
    String str = "/DEBUG/SERVO/id/";
    str += num;
    str += "/";
    Serial.println(str);
  }
  num = Serial.parseInt();
  cmds->event = num;
  if (DEBUG_SERIALCMD) {
    String str = "/DEBUG/SERVO/angle/";
    str += num;
    str += "/";
    Serial.println(str);
  }
  byte res = servoCmdEvent(cmds);
  String str = "/SERVO/CMD/";
  if (res) {
    str += "OK/";
    str += cmds->id;
    str += "/";
    str += cmds->event ;
    str += "/OK/";
  } else {
    str += "ERROR/";
    str += cmds->id;
    str += "/";
    str += cmds->event ;
    str += "/ERROR/";
  }
  Serial.println(str);
  Serial.flush();
  return res;
}

void serialEvent() {
  byte cmdStatus = 0;
  if (DEBUG_SERIALCMD) {
    Serial.print("Serial Event, Available: ");
    Serial.println(Serial.available());
  }

  while (serialWaitAvailable(2)) {
    char c = (char)Serial.read();
    if (DEBUG_SERIALCMD) {
      Serial.print("Serial Event, Available: ");
      Serial.println(Serial.available());
    }
    if (DEBUG_SERIALCMD)Serial.println(c);
    // delay(10); // este delay e para dar tempo de todos os dados chegarem na porta serial.

    if (cmdStatus == CMD_BEGIN && (c == 'L' || c == 'l')) {
      if (Serial.readStringUntil('/')) {
        cmdStatus = CMD_LASER;
        if (parseSerialLaserCmd()) cmdStatus == CMD_END;
        else cmdStatus == CMD_ERROR;
        continue;
      }
      cmdStatus = CMD_ERROR;
      break;
    } else if (cmdStatus == CMD_BEGIN && (c == 'S' || c == 's')) {
      if (Serial.readStringUntil('/')) {
        cmdStatus = CMD_SERVO;
        if (parseSerialServoCmd()) cmdStatus == CMD_END;
        else cmdStatus == CMD_ERROR;
        continue;
      }
      cmdStatus = CMD_ERROR;
      break;
    } else if (!cmdStatus && c == '/') {
      if (DEBUG_SERIALCMD) {
        Serial.println("/DEBUG/INICIO COMANDO!");
      }

      cmdStatus = CMD_BEGIN;
      continue;
    } else if (c == '\n') { // FIM DA LINHA
      if (DEBUG_SERIALCMD) {
        Serial.println("/DEBUG/FIM DE COMANDO!");
      }
      cmdStatus = CMD_END;
      break;
    }
  }
  cmdStatus = CMD_ERROR;
  //  delay(10);
}

