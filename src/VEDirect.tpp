#include "VEDirect.h"



template <class SerialType>
VEDirect<SerialType>::VEDirect(SerialType& serial, receiveCallback receive)
    : serialPort{serial}, rxCallback{receive} {}


template <class SerialType>
void VEDirect<SerialType>::setCallback(receiveCallback receive) {
    rxCallback=receive;
}


template <class SerialType>
template <typename ConfigType>
void VEDirect<SerialType>::begin(int8_t rxPin, int8_t txPin, ConfigType config) {  
  if (rxPin && txPin) {
    serialPort.begin(VEDirect_kBaud, config, rxPin, txPin);
  }
  else  {
    serialPort.begin(VEDirect_kBaud);
  }
  rxBuffer.size = 0;
}

template <class SerialType>
void VEDirect<SerialType>::update()
{
  while (serialPort.available() > 0)  {
    char rxData = serialPort.read();
    uint8_t rxCount = ved_deframe(&rxBuffer, rxData);
    if (rxCount > 0)  {
      rxBuffer.size = 0;
      uint8_t command = ved_getCommand(&rxBuffer);
      if ((command == VEDirect_kGetCommand) ||
          (command == VEDirect_kSetCommand) ||
          (command == VEDirect_kAsyncCommand))  {
        uint16_t id = ved_getId(&rxBuffer);
        // Serial.print("VE Cmd: ID is ");
        // Serial.println(id, HEX);
        switch (id)
        {
        case VEDirect_kPanelPower:
          rxCallback(id, ved_getU32(&rxBuffer));
          break;
        case VEDirect_kPanelVoltage:
        case VEDirect_kPanelCurrent:
        case VEDirect_kChargerVoltage:
        case VEDirect_kChargerCurrent:
          rxCallback(id, ved_getU16(&rxBuffer));
          break;
        case VEDirect_kDeviceState:
          rxCallback(id, ved_getU8(&rxBuffer));
          break;
        default:
          break;
        }
      }
    }
  }
}

template <class SerialType>
size_t VEDirect<SerialType>::set(uint16_t id, int32_t value) {
  ved_t txBuffer;
  ved_setCommand(&txBuffer, VEDirect_kSetCommand);
  ved_setId(&txBuffer, id);
  switch (id)   {
    case VEDirect_kBatterySense:
  case VEDirect_VoltageSetpoint:
  case VEDirect_CurrentLimit:
    ved_setU16(&txBuffer, value);
    break;
  case VEDirect_kNetworkMode:
    ved_setU8(&txBuffer, value);
    break;
  default:
    return 0;
  }
  ved_enframe(&txBuffer);
  return serialPort.write(txBuffer.data, txBuffer.size);
}

template <class SerialType>
size_t VEDirect<SerialType>::get(uint16_t id, uint16_t veCmd)
{
  ved_t txBuffer;
  ved_setCommand(&txBuffer, veCmd);
  ved_setId(&txBuffer, id);
  ved_enframe(&txBuffer);
  return serialPort.write(txBuffer.data, txBuffer.size);
}


template <class SerialType>
size_t VEDirect<SerialType>::ping()
{
  ved_t txBuffer;
  ved_setCommand(&txBuffer, VEDirect_kPingCommand);
  ved_enframe(&txBuffer);
  return serialPort.write(txBuffer.data, txBuffer.size);
}

template <class SerialType>
size_t VEDirect<SerialType>::restart()
{
  ved_t txBuffer;
  ved_setCommand(&txBuffer, VEDirect_kRestartCommand);
  ved_enframe(&txBuffer);
  return serialPort.write(txBuffer.data, txBuffer.size);
}
