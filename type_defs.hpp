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
  int16_t soakTemp;
  int16_t soakDuration;
  int16_t peakTemp;
  int16_t peakDuration;
  double  rampUpRate;
  double  rampDownRate;
  uint8_t checksum;
} Profile_t;

typedef enum REFLOW_STATE
{
  REFLOW_STATE_IDLE,
  REFLOW_STATE_PREHEAT,
  REFLOW_STATE_SOAK,
  REFLOW_STATE_REFLOW,
  REFLOW_STATE_COOL,
  REFLOW_STATE_COMPLETE,
  REFLOW_STATE_TOO_HOT,
  REFLOW_STATE_ERROR
} reflowState_t;



#endif // TYPE_DEFS_HPP
