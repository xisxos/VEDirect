#include "VEDirect.h"

// VE Direct Plug
// ┏━━     ━━┓
// ┃ 1|2|3|4 ┃
// ┗━━━━━━━━━┛
// 1. GND
// 2. VE.Direct-RX - connects to Arduino TX1
// 3. VE.Direct-TX - connects to Arduino RX1
// 4. Power +

void mpptCallback(uint16_t id, int32_t value);
VEDirect mppt(Serial1, mpptCallback);

uint16_t panelVoltage = 0;
uint16_t chargeCurrent = 0;

void setup()
{
  Serial.begin(19200);
  mppt.begin();
  // To instead open Serial1 using pins 13 (rx) and 14 (tx):
  // mppt.begin(13, 14);
}

void loop()
{
  static unsigned long secondsTimer = 0;
  mppt.update();
  unsigned long m = millis();
  if (m - secondsTimer > 1000L)
  {
    secondsTimer = m;
    mppt.ping(); // send ping every second
  }
}

void mpptCallback(uint16_t id, int32_t value)
{
  if (id == VEDirect_kPanelVoltage)
  {
    panelVoltage = value;
    Serial.print(F("Vpv : "));
    Serial.println(value * 0.01);
  }
  if (id == VEDirect_kChargerCurrent)
  {
    chargeCurrent = value;
    Serial.print(F("Ich : "));
    Serial.println(value * 0.1);
  }
}
