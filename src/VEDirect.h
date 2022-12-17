#ifndef VEDIRECT_H_
#define VEDIRECT_H_

#include "Arduino.h"
#include "ved.h"

// commeon defines
#define VEDirect_kBaud (19200)
#define VEDirect_kPingCommand (0x01)
#define VEDirect_kRestartCommand (0x06)
#define VEDirect_kGetProductIdCmd (0x04)
#define VEDirect_kGetProductIdReply (0x01)
#define VEDirect_kPingReply (0x05)
#define VEDirect_kSetCommand (0x08)
#define VEDirect_kGetCommand (0x07)
#define VEDirect_kAsyncCommand (0x0A)

// subset of register definitions from BlueSolar-HEX-protocol-MPPT.pdf
#define VEDirect_kBatterySense (0x2002)

// Solar Panel Data Registers
#define VEDirect_kPanelPower (0xEDBC)          // 0.01 | un32 | W
#define VEDirect_kPanelVoltage (0xEDBB)        // 0.01 | un16 | V
#define VEDirect_kPanelCurrent (0xEDBD)        // 0.1  | un16 | A
#define VEDirect_kPanelMaximumVoltage (0xEDB8) // 0.01 | un16 | V
#define VEDirect_kTrackerMode (0xEDB3)         //    - | un8  | -

// Load Output Data and Settings Registers
#define VEDirect_kLoadCurrent (0xEDAD)         // 0.1 | un16 | A
#define VEDirect_kLoadOffsetVoltage (0xEDAC)   // 0.01|  un8 | V
#define VEDirect_kLoadOutputControl (0xEDAB)   //
#define VEDirect_kLoadOutputVoltage (0xEDA9)   // 0.01 | un16 | V
#define VEDirect_kLoadOutputState (0xEDA8)     //    - |  un8 | -
#define VEDirect_kLoadSwitchHighLevel (0xED9D) // 0.01 | un16 | V
#define VEDirect_kLoadSwitchLowLevel (0xED9C)  // 0.01 | un16 | V
#define VEDirect_kLoadOutputOffReason (0xED91) //    - |  un8 | -
#define VEDirect_kLoadAESTimer (0xED90)        //    1 | un16 | min
// Battery settings/ partly reading / of charger 
#define VEDirect_kBatteryVoltage (0xEDEF)     //  0.01 | un8  | V

// Charger Registers
#define VEDirect_kDeviceMode (0x0200)                
#define VEDirect_kBatteryTemperature (0xEDEC)         //  0.01 | un16 | K
#define VEDirect_kChargerMaximumCurrent (0xEDDF)      //  0.1  | un16 | A
#define VEDirect_kSystemYield (0xEDDD)                //  0.01 | un32 | kWh
#define VEDirect_kUserYield (0xEDDC)                  //  0.01 | un32 | kWh
#define VEDirect_kChargerInternalTemperature (0xEDDB) //  0.01 | sn16 | C
#define VEDirect_kChargerErrorCode (0xEDDA)           //     - |  un8 | -
#define VEDirect_kChargerCurrent (0xEDD7)             //  0.1  | un16 | A
#define VEDirect_kChargerVoltage (0xEDD5)             //  0.01 | un16 | V
#define VEDirect_kAdditionalChargerStateInfo (0xEDD4) //     - | |
#define VEDirect_kYieldToday (0xEDD3)                 //  0.01 | (un32 <=1.12)) un16 | kWh
#define VEDirect_kMaximumPowerToday (0xEDD2)          //  1    | | W
#define VEDirect_kYieldYesterday (0xEDD1)             //  0.01 | | kWh
#define VEDirect_kMaximumPowerYesterday (0xEDD0)      //  1    | un16 | W
#define VEDirect_kVoltageSettingsRange (0xEDCE)       //     - | un16 | -
#define VEDirect_kHistoryVersion (0xEDCD)             //     - |  un8 | -
#define VEDirect_kStreetlightVersion (0xEDCC)         //     - |  un8 | -
#define VEDirect_kAdjustableVoltageMinimum (0x2211)   //  0.01 | un16 | V
#define VEDirect_kAdjustableVoltageMaximum (0x2212)   //  0.01 | un16 | V

#define VEDirect_kNetworkMode (0x200E)
#define VEDirect_VoltageSetpoint (0x2001)
#define VEDirect_CurrentLimit (0x2015)
#define VEDirect_kDeviceState (0x0201)

#define VEDirect_kExternalControlMode (0x05)

// Common commands
#define VEDirect_kDeviceProductId (0x0100)
#define VEDirect_kDeviceSerialNo (0x010A)
#define VEDirect_kDeviceModelName (0x010B)
#define VEDirect_kDeviceCapailities (0x0140)
#define VEDirect_kDeviceState (0x0201)

#define VEDIrect_kDeviceOffReason (0x0207)


typedef void (*receiveCallback)(uint16_t id, int32_t value);

template <class SerialType>
class VEDirect
{
public:
  VEDirect(SerialType &serial, receiveCallback receive=NULL);
  void setCallback(receiveCallback receive);
  
  template <typename ConfigType>
  void begin(int8_t rxPin = -1, int8_t txPin = -1, ConfigType config=0);
  
  void update();
  size_t set(uint16_t id, int32_t value);
  size_t get(uint16_t id, uint16_t veCmd=VEDirect_kGetCommand);
  size_t ping();
  size_t restart();

protected:
  SerialType &serialPort;
  receiveCallback rxCallback;
  ved_t rxBuffer;
};


#include <VEDirect.tpp>

#endif //  VEDIRECT_H_
