#ifndef TYPE_DEFS_HPP
#define TYPE_DEFS_HPP

uint8_t vSystemFaultBit = 0b00000000;
enum eSysFaultBits:uint8_t { 
    mcpFault,                                                       // bit 0 = (1) MCP9600 Hard Fault
    eSysFaultBit1,                                                  // bit 1 = (2) UNDEFINED
    eSysFaultBit2,                                                  // bit 2 = (4) UNDEFINED
    eSysFaultBit3,                                                  // bit 3 = (8) UNDEFINED
    eSysFaultBit4,                                                  // bit 4 = (16) UNDEFINED
    eSysFaultBit5,                                                  // bit 5 = (32) UNDEFINED
    eSysFaultBit6,                                                  // bit 6 = (64) UNDEFINED
    eSysFaultBit7                                                   // bit 7 = (128) UNDEFINED
};

// data type for the values used in the reflow profile
typedef struct profileValues_s {
  uint8_t soakTemp;
  int16_t soakDuration;
  uint8_t peakTemp;
  int16_t peakDuration;
  double  rampUpRate;
  double  rampDownRate;
  uint8_t checksum;
} Profile_t;

typedef enum reflowState_e:uint8_t {
    reflowIdle,
    reflowPreheat,
    reflowSoak,
    reflowNow,
    reflowCool,
    reflowComplete,
    reflowOverHeat,
    reflowError
} reflowState_t;



#endif // TYPE_DEFS_HPP
