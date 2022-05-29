#include <Arduino.h>
#include <Wire.h>
//#include "lib/CA9500.hpp"
//CA9500 _i2c;

#include "lib/PID_v2.h"

#include "type_defs.hpp"
#include "hwSetup.hpp"
#include "lib/crc8.hpp"
#include "lib/TFT_ILI9334.h"

#include <EEPROMEx.h>

#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

Adafruit_SSD1306 oled[2] = { Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
                             Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) };

#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_MCP9600.h>
Adafruit_MCP9600 mcp;

// set mcp address
#define mcpHex (0x67)

#ifndef bitToggle
    #define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#endif

// set the temp and output variables
float _tInput = 0, _tSetPoint = 150, _Output = 0, _tGap = 0;

// Define the standard, aggressive, and conservative Tuning Parameters
float Kp = 2.0, Ki = 5.0, Kd = 1.0;
float aggKp = 4.0, aggKi = 0.2, aggKd = 1.0;
float consKp = 1.0, consKi = 0.05, consKd = 0.25;

// set the time proportional settings
const uint16_t _timeWindow = 5000; // ms
uint16_t _timeStart;

//Specify the links and initial tuning parameters
//PID_v2 reflowPID(Kp, Ki, Kd, PID::Direct);
PID_v2 reflowPID(consKp, consKi, consKd, PID::Direct);

void setup()
{
    _timeStart = millis();

    // SSD1306_SWITCHCAPVCC = generate oled voltage from 3.3V internally
    oled[0].begin(SSD1306_SWITCHCAPVCC, 0x3c);
    //oled[1].begin(SSD1306_SWITCHCAPVCC, 0x3d);


    //if ( not mcp.begin(mcpHex) ) { bitSet(vSystemFaultBit,eSysFaultBits::mcpFault); }
    mcp.begin(mcpHex);
    mcp.setADCresolution(MCP9600_ADCRESOLUTION_18);
    mcp.setThermocoupleType(MCP9600_TYPE_K);
    mcp.setFilterCoefficient(3);
    mcp.enable(true);

    // tell the PID to range between 0 and the full window size
    myPID.SetOutputLimits(0, _timeWindow);

    // get the first temp read after a delay of 5000ms to stabilize the thermocouple
    // then start the PID with the parameters
    delay(5000);
    _tInput = mcp.readThermocouple();
    reflowPID.Start(_tInput, _Output, _tSetPoint);
    while(1) {
    // Clear the buffer
            oled[0].clearDisplay();
            oled[0].setTextSize(2);
            oled[0].setTextSize(3,2);
            oled[0].setTextColor(SSD1306_WHITE);
            //oled[0].setTextColor(SSD1306_BLACK, SSD1306_WHITE);
            oled[0].setCursor(0,0);
    oledSetCursorCenterMode(_tInput, 1, 1, 0, -10);
    oled[0].println(_tInput);
    oled[0].display();
    delay(1000);
    }

}

void loop()
{
    _tInput = mcp.readThermocouple();
    _tGap = abs(reflowPID.GetSetpoint() - _tInput);

    if ( _tGap > 10 ) {
        // we're close to setpoint, use conservative tuning parameters
        reflowPID.SetTunings(consKp, consKi, consKd);
    } else {
        // we're far from setpoint, use aggressive tuning parameters
        reflowPID.SetTunings(aggKp, aggKi, aggKd);
    }

    _Output = reflowPID.Run(_tInput);
    
    // adjust time until it is time
    while (millis() - _timeStart > _timeWindow) {
        // time to shift the Relay Window
        _timeStart += _timeWindow;
    }

    if (millis() - _timeStart >= _Output ) {
            // heater on
    } else {
            // heater off
    }

}
void oledSetCursorCenterMode(String _text, uint8_t _instance=0, uint8_t _mode=0, int8_t _horOffset=0, int8_t _verOffset=0 );
void oledSetCursorCenterMode(String _text, uint8_t _instance, uint8_t _mode, int8_t _horOffset, int8_t _verOffset ) {
    int16_t x1;
    int16_t y1;
    uint16_t width;
    uint16_t height;

    // turn off wrap before finding L/R bounds
    oled[_instance].setTextWrap(false);
    oled[_instance].getTextBounds(_text, 0, 0, &x1, &y1, &width, &height);
    // this will dynamically change the font smaller if too big to fit with no wrap
    while (width > SCREEN_WIDTH) {
        _tSize--;
        oled[_instance].setTextSize(_tSize);
        oled[_instance].getTextBounds(_text, 0, 0, &x1, &y1, &width, &height);
    }
    oled[_instance].setTextWrap(true);

    // set cursor so text is on horizontal and vertical centers minus any offset
    switch (_mode) {
        case 1:
            // horizontal only
            oled[_instance].setCursor( ((SCREEN_WIDTH - width) / 2)-horOffset, 0);
            return;
        case 2:
            // vertical only
            oled[_instance].setCursor( 0, ((SCREEN_HEIGHT - height) / 2)-verOffset );
            return;
        default:
            //full center
            oled[_instance].setCursor( ((SCREEN_WIDTH - width) / 2)-horOffset, ((SCREEN_HEIGHT - height) / 2)-verOffset );
            return;
     }
}
