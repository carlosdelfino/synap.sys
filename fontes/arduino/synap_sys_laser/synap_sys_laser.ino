
#include <avr/interrupt.h>
#include <avr/io.h>
#include <Servo.h>
#include "debug.h"
#include "parameters.h"
#include "serialcmd.h"

long lastTimeTempHeatSink, lastTimeTempRoom, lastTimeTempInternal;

void setup() {
  Serial.begin(9600);

  setupSerialCmd();
  setupLasers() ;
  setupServos() ;

  lastTimeTempHeatSink = lastTimeTempRoom = lastTimeTempInternal = millis();
}

void loop() {
  long time = millis();
  if ((time - lastTimeTempHeatSink) > DELAY_TEMP_HEAT_SINK) {
    Serial.print("/temp/heatsink/");
    double temp = readHeatSinkTemp();
    Serial.print(temp);
    Serial.println("/");
    lastTimeTempHeatSink = time;
  }
  delay(DELAY_TEMP_INTER_READ);

  time = millis();
  if ((time - lastTimeTempRoom) > DELAY_TEMP_ROOM) {
    Serial.print("/temp/room/");

    double temp = readRoomTemp();
    Serial.print(temp);
    Serial.println("/");

    analogReference(DEFAULT);
    lastTimeTempRoom = time;
  }
  delay(DELAY_TEMP_INTER_READ);
/*
// estou tendo problemas em fazer a leitura interna, porque uso com 1.1V
// e ao trocar a tensao de referencia toda as leituras em tensoes diferentes param
// mesmo dando o tempo certo para retornar a tensao certa.
  time = millis();
  if ((time - lastTimeTempInternal) > DELAY_TEMP_INTERNAL) {
    Serial.print("/temp/internal/");
    Serial.print(readInternalTemp());
    Serial.println("/");
    lastTimeTempInternal = time;
  }
  delay(DELAY_TEMP_INTER_READ);
*/

  delay(500);
}

double readRoomTemp() {
#if (TEMP_AREF == (DEFAULT))  // para leitura com 5V de referencia
  ADMUX =  _BV(REFS0) ; // 5V (VCC)
  const double fator = .48828125;
#elif (TEMP_AREF == (INTERNAL))  // para leitura com 1.1V de referencia
  ADMUX = _BV(REFS1) | _BV(REFS0); // 1.1V (VCC)
  const double fator  = .107421875;
#else
  ADMUX = _BV(REFS1) | _BV(REFS0); // 1.1V, mas esta aqui nao ' configurado
  const double fator = 0;
#endif
  ADCSRB = 0;
  ADCSRA |= _BV(ADEN);  // enable the ADC

  ADMUX |= _BV(MUX0); // porta A1
  ADCSRA |= _BV(ADEN);  // enable the ADC
  delay(20);

  // descarto a primeira leitura
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA, ADSC));

  double temp = 0;
  byte mean = 0;
  for (; mean < MEAN_TEMP_INTERNAL; mean++) {
    ADCSRA |= _BV(ADSC);
    while (bit_is_set(ADCSRA, ADSC));
    byte l = ADCL;
    int h = ADCH;
    temp += (( h << 8) | l);
    delay(2);
  }
  temp /= mean;

  temp  *= fator;
  return temp;
}

/*
 * baseado no codigo: http://playground.arduino.cc/ComponentLib/Thermistor2
 */
double readHeatSinkTemp() {
  ADMUX =  _BV(REFS0); // nao precisa setar o MUX porque todos sao 0000, porta A0
  ADCSRA |= _BV(ADEN);  // enable the ADC
  delay(20);

  //descarto a primeira leitura
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA, ADSC));

  double temp = 0;
  byte mean = 0;
  for (; mean < MEAN_TEMP_INTERNAL; mean++) {
    ADCSRA |= _BV(ADSC);
    while (bit_is_set(ADCSRA, ADSC));
    byte l = ADCL;
    int h = ADCH;
    temp += (( h << 8) | l);
    delay(2);
  }
  temp /= mean;
  
  temp = log(10000.0 * (1024.0 / temp - 1));
//  temp = log(10000.0 / (1024.0 / temp - 1)); // for pull-up configuration
  temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * temp * temp )) * temp );
  temp -= 273.15;            // Convert Kelvin to Celcius
 
  return temp;
}

//https://code.google.com/p/tinkerit/wiki/SecretThermometer
double readInternalTemp() {
  ADMUX = _BV(REFS1) | _BV(REFS0) |  _BV(MUX3);

  ADCSRA |= _BV(ADEN);  // enable the ADC

  delay(20);

  // descarto a primeira leitura;
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA, ADSC));

  double temp = 0;
  byte mean = 0;
  for (; mean < MEAN_TEMP_INTERNAL; mean++) {
    ADCSRA |= _BV(ADSC);
    while (bit_is_set(ADCSRA, ADSC));
    int l = ADCL;
    int h = ADCH;
    temp += (( h << 8) | l);
    delay(2);
  }
  temp /= mean;

  temp = (temp - TEMP_INTERNAL_TOS) / TEMP_INTERNAL_K;
  return temp;
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
