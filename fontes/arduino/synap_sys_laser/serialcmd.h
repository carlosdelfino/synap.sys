#ifndef SERIALCOMMAND_H
#define SERIALCOMMAND_H

#include "parameters.h" 

#line 5

#define CMD_BEGIN        B00000001
#define CMD_END          B00000001

#define CMD_SERVO        B00100000
#define CMD_SERVO_NUM_OK B00100001
#define CMD_SERVO_OK     B00100010
#define CMD_SERVO_FIM    B00111111

#define CMD_LASER        B01000000
#define CMD_LASER_NUM_OK B01000001
#define CMD_LASER_OK     B01000010
#define CMD_LASER_FIM    B01111111

struct Cmd_t {
  uint8_t id;
  uint8_t event;
} ;

typedef struct Cmd_t * Cmd;

void attachServo(Cmd);
byte laserCmdEvent(Cmd cmds);
byte servoCmdEvent(Cmd cmds);

void setupSerialCmd();

#endif
