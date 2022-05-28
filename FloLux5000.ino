#include <PID_v1.h>




#include <Arduino.h>
#include <Wire.h>
//#include "lib/CA9500.hpp"
//CA9500 _i2c;

#include "type_defs.hpp"
#include "hwSetup.hpp"
#include "lib/crc8.hpp"
#include "lib/TFT_ILI9334.h"

#include <EEPROMEx.h>

#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_MCP9600.h>
Adafruit_MCP9600 mcp;

#define mcpHex (0x67)

#ifndef bitToggle
    #define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#endif

uint16_t _tSetPoint, _Output;
float _tInput;

//Specify the links and initial tuning parameters
PID tPID(&_tInput, &_Output, &_tSetPoint,2,5,1, DIRECT);

void setup()
{
    if ( not mcp.begin(mcpHex) ) { bitSet(vSystemFaultBit,eSysFaultBits::mcpFault); }
    //mcp.begin(mcpHex);
    mcp.setADCresolution(MCP9600_ADCRESOLUTION_18);
    mcp.setThermocoupleType(MCP9600_TYPE_K);
    mcp.setFilterCoefficient(3);
    mcp.enable(true);

    _tInput = mcp.readThermocouple();
    _tSetPoint = 150;

    tPID.SetMode(AUTOMATIC);
}

void loop()
{
    tPID.Compute();
    
}
