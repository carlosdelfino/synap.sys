#include <Servo.h>
#include "debug.h"
#include "parameters.h"
#include "serialcmd.h"

void setup() {
  Serial.begin(9600);
  
  setupSerialCmd();
  setupLasers() ;
  setupServos() ;

}

void loop() {
  delay(100);
}

/********************
setup do laser
*/
void setupLasers() {

  Serial.println("\n/init/lasers/check");
  String str;
  
  Cmd cmds = (Cmd)malloc(sizeof(Cmd));

  for (byte b = 0; b < NUM_LASERS; b++) {

    cmds->id = b;

    pinMode(LASERS[cmds->id], OUTPUT);

    cmds->event = HIGH;
    laserCmdEvent(cmds);

    delay(20);

    cmds->event = !cmds->event;
    laserCmdEvent(cmds);

    if (DEBUG_INIT) {
      str = "/DEBUG/LASER/";
      str += b;
      Serial.println(str);
    }
  }
  
  free(cmds);

  Serial.println("\n/init/lasers/ok");
}

/********************
setup do Servos
*/
void setupServos() {
  Serial.println("\n/init/servos/check");
  String str;
  
  Cmd cmds = (Cmd)malloc(sizeof(Cmd));
  
  for (byte b = 0; b < NUM_SERVOS; b++) {

    cmds->id = b;

    attachServo(cmds);

    cmds->event = MAX_SERVO_ANGLE;
    servoCmdEvent(cmds);
    delay(SERVO_DELAY_TEST);
    cmds->event = MIDLE_SERVO_ANGLE;
    servoCmdEvent(cmds);
    delay(SERVO_DELAY_TEST);
    cmds->event = MIN_SERVO_ANGLE;
    servoCmdEvent(cmds);
    delay(SERVO_DELAY_TEST);

    if (DEBUG_INIT) {
      str = "/DEBUG/SERVO/";
      str += cmds->id;
      str += "/";
      str += cmds->event;
      Serial.println(str);
    }
  }
  
  free(cmds);
  
  Serial.println("\n/init/lasers/ok");
}
