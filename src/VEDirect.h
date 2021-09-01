#ifndef VEDIRECT_H_
#define VEDIRECT_H_

#include "Arduino.h"
#include "ved.h"

enum {
  VEDirect_kBatterySense = 0x2002,
  VEDirect_kPanelPower = 0xEDBC,
  VEDirect_kPanelVoltage = 0xEDBB,
  VEDirect_kBatteryVoltage = 0xEDD5,
};

typedef void (*receiveCallback)(uint16_t id, int32_t value);

class VEDirect {
public:
  VEDirect(HardwareSerial &serial, receiveCallback receive);
  void begin();
  void update();
  size_t set(uint16_t id, int32_t value);
  size_t get(uint16_t id);
  size_t ping();
private:
  HardwareSerial &serialPort;
  receiveCallback rxCallback;
  ved_t rxBuffer;
};

#endif //  VEDIRECT_H_