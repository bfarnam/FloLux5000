#include <Arduino.h>
#include <Wire.h>
#include "CA9500.hpp"
CA9500 _i2c;

#include "PID_v2.h"

#include "type_defs.hpp"
#include "hwSetup.hpp"
//#include "lib/crc8.hpp"
//#include "lib/TFT_ILI9341.h"

//#include <EEPROMEx.h>

#include "Adafruit_SSD1306.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

const uint8_t oAddr[2] = {0x3c, 0x3d};

// The adafruit library is messed up and you can't substantiate two seperate oleds using the new constructors into an array
//Adafruit_SSD1306 oled[2] = { Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET), Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) };
// the workaround is to use the old constructor, but you can't set to 128x63 or it fails.
// so the other workaround is to leave it set to 128x64 and then scale the fonts mannually x and y
// and then you have to modify the library and manuualy set the i2c speed to 400k.... this cost me MANY hours to debug
Adafruit_SSD1306 oled[2] = { Adafruit_SSD1306(), Adafruit_SSD1306() };

// font x and y scaling
uint8_t _tSizeX;
uint8_t _tSizeY;

// declare function for auto scaling font width and auto placing the center
// becuase of the issues above with the constructor, a vertical offset of 16 is used to place
// the font back on the screen
void oledSetCursorCenterMode(String _text, uint8_t _instance=0, uint8_t _mode=0, int8_t _horOffset=0, int8_t _verOffset=0 );

#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_MCP9600.h>
Adafruit_MCP9600 mcp;

// set mcp address
const uint8_t mcpHex (0x67);

#ifndef bitToggle
    #define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#endif

// set the temp and output variables
double _tInput = 0, _tSetPoint = 150, _Output = 0, _tGap = 0;

// Define the standard, aggressive, and conservative Tuning Parameters
double Kp = 2.0, Ki = 5.0, Kd = 1.0;
double aggKp = 4.0, aggKi = 0.2, aggKd = 1.0;
double consKp = 1.0, consKi = 0.05, consKd = 0.25;

// set the time proportional settings
const uint16_t _timeWindow = 5000; // ms
uint16_t _timeStart;

//Specify the links and initial tuning parameters
//PID_v2 reflowPID(Kp, Ki, Kd, PID::Direct);
//PID_v2 reflowPID(consKp, consKi, consKd, PID::Direct);

void setup()
{
  Serial.begin(115200);
    _timeStart = millis();
/*
    _i2c.begin(0x27, Wire);
    bool pMode[8] = {OUTPUT,OUTPUT,OUTPUT,INPUT,OUTPUT,OUTPUT,OUTPUT,OUTPUT};
    _i2c.pinMode(pMode);
    bool pState[8] = {LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW};
    _i2c.digitalWrite(pState);
*/
    // SSD1306_SWITCHCAPVCC = generate oled voltage from 3.3V internally
    // initiate the oleds and display thier number on the screen
    for (uint8_t x = 0; x < 2; x++) {
        Serial.print(F("begin oled instance "));
        Serial.println(String(x));
        Serial.println(oled[x].begin(SSD1306_SWITCHCAPVCC, oAddr[x],false,true));
        delay(2000);
        oled[x].clearDisplay();
        // remember the scale issue cause the GFX lib thinks it is 32 tall
        oled[x].setTextSize(12,2); //x,y x = hor y = ver
        oled[x].setTextColor(SSD1306_WHITE);
        oled[x].setCursor(0,0);
        oledSetCursorCenterMode(String(x),x,0,0,16);
        oled[x].print(String(x));
        oled[x].display();
    }
    // give you time to read the numbers....
    delay(2000);
    
    //if ( not mcp.begin(mcpHex) ) { bitSet(vSystemFaultBit,eSysFaultBits::mcpFault); }
    if ( not mcp.begin(mcpHex) ) { 
      Serial.print(F("mcp instance FAILED"));
      while(1);
    }
    //mcp.begin(mcpHex);
    mcp.setADCresolution(MCP9600_ADCRESOLUTION_18);
    mcp.setThermocoupleType(MCP9600_TYPE_K);
    mcp.setFilterCoefficient(3);
    mcp.enable(true);

    // tell the PID to range between 0 and the full window size
//    reflowPID.SetOutputLimits(0, _timeWindow);

    // get the first temp read after a delay of 1000ms to stabilize the thermocouple
    // then start the PID with the parameters
    delay(1000);
    _tInput = mcp.readThermocouple();
    
//    reflowPID.Start(_tInput, _Output, _tSetPoint);

    // this is test code
    while(1) { 
        DisplayTemp("TEMP", String(mcp.readThermocouple()), 4, 3, 0);
        DisplayTemp("SETPOINT", String(_tInput+1800), 4, 3, 1);
        _tInput += 1.1;
    }

}

void loop()
{

//    _tInput = mcp.readThermocouple();
//    _tGap = abs(reflowPID.GetSetpoint() - _tInput);

//    if ( _tGap > 10 ) {
        // we're close to setpoint, use conservative tuning parameters
//        reflowPID.SetTunings(consKp, consKi, consKd);
//    } else {
        // we're far from setpoint, use aggressive tuning parameters
//        reflowPID.SetTunings(aggKp, aggKi, aggKd);
//    }

//    _Output = reflowPID.Run(_tInput);
    
    // adjust time until it is time
    while (millis() - _timeStart > _timeWindow) {
        // time to shift the Relay Window
        _timeStart += _timeWindow;
    }

    if (millis() - _timeStart >= _Output ) {
            // heater on
//            _i2c.digitalWrite(0,HIGH);
//            _i2c.digitalWrite(1,HIGH);
    } else {
            // heater off
//            _i2c.digitalWrite(0,LOW);
//            _i2c.digitalWrite(1,LOW);
    }

}

void DisplayTemp(String _banner, String _temp, uint8_t _sizeX, uint8_t _sizeY, uint8_t _instance) {
    _tSizeX = 3;
    _tSizeY = 1;
    oled[_instance].clearDisplay();
    // first set banner size and posisition
    oled[_instance].setTextSize(_tSizeX,_tSizeY); //x,y x = hor y = ver
    oled[_instance].setTextColor(SSD1306_WHITE);
    oled[_instance].setCursor(0,0);
    // (string, instance, mode, horiz offset, vert offset)
    oledSetCursorCenterMode(_banner, _instance, 1);
    oled[_instance].println(_banner);
    
    // now set temp display size and posisiton
    _tSizeX = _sizeX;
    _tSizeY = _sizeY;
    oled[_instance].setTextSize(_tSizeX,_tSizeY); //x,y x = hor y = ver
    //oled[0].setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    oled[_instance].setTextColor(SSD1306_WHITE);
    oled[_instance].setCursor(0,0);
    oledSetCursorCenterMode(String(_temp), _instance, 0, 0, 10);
    oled[_instance].print(String(_temp));
    oled[_instance].display();
}

// _text = text string (required)
// instance = OLED instance in and array 0 to xx
// mode = 1 horizontal center, 2 vertical center, x full center
// offset is + or - from center.  + is up or right, - is down or left
// returns nothing BUT sets the cursor position
void oledSetCursorCenterMode(String _text, uint8_t _instance, uint8_t _mode, int8_t _horizontalOffset, int8_t _verticalOffset ) {
    int16_t x1;
    int16_t y1;
    uint16_t width;
    uint16_t height;

    // turn off wrap before finding L/R bounds
    oled[_instance].setTextWrap(false);
    oled[_instance].getTextBounds(_text, 0, 0, &x1, &y1, &width, &height);
    // this will dynamically change the font smaller if too big to fit with no wrap
    while (width > SCREEN_WIDTH) {
        _tSizeX--;
        oled[_instance].setTextSize(_tSizeX,_tSizeY);
        oled[_instance].getTextBounds(_text, 0, 0, &x1, &y1, &width, &height);
    }
    oled[_instance].setTextWrap(true);

    // set cursor so text is on horizontal and vertical centers minus any offset
    switch (_mode) {
        case 1:
            // horizontal only
            oled[_instance].setCursor( ((SCREEN_WIDTH - width) / 2) - _horizontalOffset, 0);
            return;
        case 2:
            // vertical only
            oled[_instance].setCursor( 0, ((SCREEN_HEIGHT - height) / 2) - _verticalOffset );
            return;
        default:
            // full center
            oled[_instance].setCursor( ((SCREEN_WIDTH - width) / 2) - _horizontalOffset, ((SCREEN_HEIGHT - height) / 2) - _verticalOffset );
            return;
     }
}
