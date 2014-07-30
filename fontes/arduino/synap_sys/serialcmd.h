#ifndef SERIALCOMMAND_H
#define SERIALCOMMAND_H
#include "parameters.h" 


#define DEBUG_SERIALCMD (D_L_SERIALCMD == (DEBUG & D_L_SERIALCMD ))

#define CMD_OK         B00000001

#define CMD_LED        B00010000
#define CMD_LED_NUM_OK B00010001
#define CMD_LED_EVENT  B00010010
#define CMD_LED_FIM    B00011111

struct LedCmd_t {
  bool event;
  byte led;
} ;
typedef struct LedCmd_t * LedCmd;

void serialEventLedCmd(LedCmd ledcmd, char c, byte *cmd);
void setupLeds();

const byte NUM_LEDS  = sizeof(LEDS) / sizeof(*LEDS);

#endif
