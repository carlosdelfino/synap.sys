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

Servo servosObj[NUM_SERVOS];

Cmd cmds = (Cmd)malloc(sizeof(Cmd));

void setupSerialCmd() {
  cmds->event = 0;
  cmds->id = 99;
}

void attachServo(Cmd cmd) {
  servosObj[cmd->id].attach(SERVOS[cmd->id]);
}


byte servoCmdEvent(Cmd cmd) {
  bool res = cmd->id >= 0 && cmd->id < NUM_SERVOS;
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

  if (res)
    servosObj[cmd->id].write(cmd->event);
  else if (DEBUG_SERIALCMD) {
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
  bool res = cmds->id >= 0 && cmds->id < NUM_LASERS;
  if (DEBUG_SERIALCMD) {
    String str = "/DEBUG/LASER/exec/event/";
    str += cmds->event;
    str += "/";
    str += cmds->id;
    str += "/";
    str += LASERS[cmds->id];
    Serial.println(str);
  }

  if (res)
    digitalWrite(LASERS[cmds->id], cmds->event);
  else if (DEBUG_SERIALCMD) {
    String str = "/DEBUG/LASER/exec/event/ERROR/";
    str += cmds->event;
    str += "/";
    str += cmds->id;
    str += "/";
    str += LASERS[cmds->id];
    str += "/ERROR";
    Serial.println(str);
  }

  return res;
}



void parseSerialLaserCmd(Cmd cmds, char c, byte * cmdStatus_p) {
  byte cmdStatus = (*cmdStatus_p);
  (*cmdStatus_p) = 0;

  if (DEBUG_SERIALCMD) {
    String str = "/DEBUG/LASER/parseSerialLaserCmd/";
    str += cmds->id;
    str += "/";
    str += cmds->event;
    str += "/";
    str += cmdStatus;
    str += "/";
    str += c;

    Serial.println(str);
  }

  if (cmdStatus == CMD_LASER_OK) {
    byte res = laserCmdEvent(cmds);
    String str = "/LASER/CMD/";
    if (res) {
      str += "OK/";
      str += cmds->event;
      str += "/";
      str += cmds->id;
      str += "/OK/";
    }
  } else if (cmdStatus == CMD_LASER && c == '/') {
    byte  num = Serial.parseInt();
    cmds->id = num;
    if (DEBUG_SERIALCMD) {
      String str = "/DEBUG/LASER/num/";
      str += num;
      str += "/";
      Serial.println(str);
    }
    (*cmdStatus_p) =  CMD_LASER_NUM_OK;

  } else if (cmdStatus == CMD_LASER_NUM_OK && c == '/') {
    if (Serial.available() >= 2) {
      c = (char)Serial.read();
      c = (char)Serial.read();
    } else return;

    cmds->event = (c == 'N' || c == 'n');
    (*cmdStatus_p) = CMD_LASER_OK;
  }
}

void parseSerialServoCmd(Cmd cmds, char c, byte * cmdStatus_p) {
  byte cmdStatus = (*cmdStatus_p);
  (*cmdStatus_p) = 0;

  if (DEBUG_SERIALCMD) {
    String str = "/DEBUG/SERVO/parseSerialServoCmd/";
    str += cmds->id;
    str += "/";
    str += cmds->event;
    str += "/";
    str += cmdStatus;
    str += "/";
    str += c;

    Serial.println(str);
  }


  if (cmdStatus == CMD_SERVO_SET_ANGLE_OK) {
    byte res = servoCmdEvent(cmds);
    String str = "/DEBUG/SERVO/CMD_SET_ANGLE_OK/";
    if (res) {
      str += "OK/";
      str += cmds->event ;
      str += "/";
      str += cmds->id;
      str += "/OK/";
    } else {
      str += "ERROR/";
      str += cmds->event ;
      str += "/";
      str += cmds->id;
      str += "/ERROR/";
    }
    Serial.println(str);
  } else if (cmdStatus == CMD_SERVO_GET_ANGLE_OK) {
    byte angle = servosObj[cmds->id].read();
    if (DEBUG_SERIALCMD) {
      String str = "/DEBUG/SERVO/CMD_SERVO_GET_ANGLE_OK/id/";
      str += cmds->id;
      str += "/angle/";
      str += angle;
      str += "/";
      Serial.println(str);
    }
    Serial.print("/servo/angle/");
    Serial.print(angle);
    Serial.println("/");

  } else if (cmdStatus == CMD_SERVO && c == '/') {

    byte  num = Serial.parseInt();
    cmds->id = num;
    if (DEBUG_SERIALCMD) {
      String str = "/DEBUG/SERVO/CMD_SERVO/id/";
      str += num;
      str += "/";
      Serial.println(str);
    }
    (*cmdStatus_p) =  CMD_SERVO_NUM_OK;
  } else if (cmdStatus == CMD_SERVO_NUM_OK && c == '/') {
    c = Serial.peek();
    if (DEBUG_SERIALCMD) {
      String str = "/DEBUG/SERVO/CMD_SERVO_NUM_OK/";
      Serial.println(str);
    }
    if ( c == 'A' || c == 'a') {
      while (Serial.available() && c != '/') {
        c = Serial.read();
        if (DEBUG_SERIALCMD) {
          Serial.println(c);
        }
      }
      if (DEBUG_SERIALCMD) {
        String str = "/DEBUG/CMD_SERVO_NUM_OK/get angle/";
        Serial.println(str);
      }
      (*cmdStatus_p) =  CMD_SERVO_GET_ANGLE_OK;
    } else {
      byte  num = Serial.parseInt();
      cmds->event = num;
      if (DEBUG_SERIALCMD) {
        String str = "/DEBUG/CMD_SERVO_NUM_OK/set angle/";
        str += num;
        str += "/";
        Serial.println(str);
      }
      (*cmdStatus_p) = CMD_SERVO_SET_ANGLE_OK;
      parseSerialServoCmd(cmds,c,cmdStatus_p);
    }
  }
}


void serialEvent() {
  static byte cmdStatus = 0;
  if (DEBUG_SERIALCMD)Serial.println("Serial Event");

  while (Serial.available()) {
    char c = (char)Serial.read();

    if (DEBUG_SERIALCMD)Serial.println(c);
    delay(10); // este delay e para dar tempo de todos os dados chegarem na porta serial.

    if (!cmdStatus && c == '/') {
      if (DEBUG_SERIALCMD) {
        Serial.println("/DEBUG//INICIO COMANDO!");
      }

      cmdStatus = CMD_BEGIN;
      continue;
    } else if (cmdStatus == CMD_BEGIN && (c == 'L' || c == 'l')) {
      if (Serial.available() >= 4) {//LASER
        c = (char)Serial.read();
        c = (char)Serial.read();
        c = (char)Serial.read();
        c = (char)Serial.read();
      } else break;
      cmdStatus = CMD_LASER;
      continue;
    } else if (cmdStatus == CMD_BEGIN && (c == 'S' || c == 's')) {
      if (Serial.available() >= 4) {//SERVO
        c = (char)Serial.read();
        c = (char)Serial.read();
        c = (char)Serial.read();
        c = (char)Serial.read();
      } else break;
      if (DEBUG_SERIALCMD) {
        Serial.println("/DEBUG//Servo COMANDO!");
      }
      cmdStatus = CMD_SERVO;
      continue;
    } else if ((cmdStatus >= CMD_LASER && cmdStatus <= CMD_LASER_FIM)) { // achei o inicio de um comando Laser

      if (DEBUG_SERIALCMD) {
        String str = "/DEBUG/LASER/CMD/call_parseSerialLaserCmd/";
        str += cmdStatus;
        str += "/";
        str += c;
        Serial.println(str);
      }
      parseSerialLaserCmd(cmds, c, &cmdStatus);
      if (DEBUG_SERIALCMD) {
        String str = "/DEBUG/LASER/CMD/return_parseSerialLaserCmd/";
        str += cmds->event;
        str += "/";
        str += cmds->id;
        str += "/";
        str += LASERS[cmds->id];
        str += "/";
        str += cmdStatus;
        Serial.println(str);
      }

    } else if ((cmdStatus >= CMD_SERVO && cmdStatus <= CMD_SERVO_FIM)) { // achei o inicio de um comando Laser
      if (DEBUG_SERIALCMD) {
        String str = "/DEBUG/SERVO/CMD/call_parseSerialServoCmd/";
        str += cmdStatus;
        str += "/";
        str += c;
        Serial.println(str);
      }
      parseSerialServoCmd(cmds, c, &cmdStatus);
      if (DEBUG_SERIALCMD) {
        String str = "/DEBUG/SERVO/CMD/return_parseSerialServoCmd/";
        str += cmds->event;
        str += "/";
        str += cmds->id;
        str += "/";
        str += SERVOS[cmds->id];
        str += "/";
        str += cmdStatus;
        Serial.println(str);
      }
    } else if (c == '\n') { // FIM DA LINHA
      if (DEBUG_SERIALCMD) {
        Serial.println("/DEBUG/FIM DE COMANDO!");
      }
      cmdStatus = 0;
      break;
    }
  }
  cmdStatus = 0;
  delay(10);
}


