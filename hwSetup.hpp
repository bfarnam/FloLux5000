#ifndef HWSETUP_HPP
#define HWSETUP_HPP

/*
 * ATMEGA358P PORT MAPPING
 *
 * D8 / PB0 (8) - ICP
 * D9 / PB1 (9) - OC1A                                  TFT_DC
 * PB2 (10) - SS0/OC1B/PTCXY                            TFT_CS
 * PB3 (11) - MOSI0/TXD1/OC2A/PTCXY **** DO NOT USE     MOSI
 * PB4 (12) - MISO0/RXD1/PTCXY **** DO NOT USE          MISO
 * PB5 (13) - PTCXY/XCK1/SCK0 **** DO NOT USE           SCK
 * PB6 - XTAL **** DO NOT USE
 * PB7 - XTAL **** DO NOT USE
 *
 * D14 / PC0 (A0) - NOT USED
 * D15 / PC1 (A1) - NOT USED
 * D16 / PC2 (A2) - NOT USED
 * D17 / PC3 (A3) - NOT USED
 * D18 / PC4 (A4) - I2C SDA (Serial DAta)
 * D19 / PC5 (A5) - I2C SCL (Serial CLock)
 * PC6 - RESET **** DO NOT USE
 *
 * PD0 - RXI **** DO NOT USE
 * PD1 - TXO **** DO NOT USE
 * D2 / PD2 (2) - INT0 - NOT USED
 * D3 / PD3 (3) - INT1 - NOT USED
 * D4 / PD4 (4) -                                       FAN
 * D5 / PD5 (5) -                                       BOTTOM HEAT
 * D6 / PD6 (6) -                                       TOP HEAT
 * D7 / PD7 (7) -                                       TFT_RST
 *
 * PE0 (22) - SDA1/ICP4/ACO/PTCXY - NOT AVAILABLE
 * PE1 (23) - SCL1/T4/PTCXY - NOT AVAILABLE
 * PE2 (A6/20) - ADC6/PTCY/ICP3/SS1 - NOT USED
 * PE3 (A7/21) - ADC7/PTCY/T3/MOSI1 - NOT USED
*/

/*
 * The burst firing mode is preferred for resistive loads, which can provide a uniform power output 
 * to the heater and can extend the life time of the heater. The firing signal is a series of short
 * pulses distributed over one hundred AC line cycles. The duration of each pulse equals to one AC
 * line cycle (16.67 milliseconds for 60Hz or 20 milliseconds for 50 Hz, automatically detected 
 * by the regulator). 
 * 
 * Note1:
 * 
 * The burst rate will between 1 and 100 pulses for each 100 AC cycles. The pulse width equals to one AC cycle. It is automatically adjusted for 50 or 60 Hz.
 * Cycle time: 1-99 seconds.
 */

#define msPerSine  83.333                           // for 60Hz US = 83.3333ms per sinusoid
//#define msPerSine 100                             // for 50Hz = 100ms per sinusoid

// for user_setup.h in tft libraries
#define TFT_CS   10                                 // Chip select control pin
#define TFT_DC   9                                  // Data Command control pin
#define TFT_RST  7                                  // Reset pin (could connect to Arduino RESET pin)

// Define an "offset" to subtract from the max temp so that the temp target fits in 1 byte
#define tempOffset 30

// ***** PID PARAMETERS *****
// ***** PRE-HEAT STAGE *****
float PID_KP_PREHEAT         = 300;
float PID_KI_PREHEAT         = 0.05;
float PID_KD_PREHEAT         = 350; 
// ***** SOAKING STAGE *****
float PID_KP_SOAK            = 300;
float PID_KI_SOAK            = 0.05;
float PID_KD_SOAK            = 350;
// ***** REFLOW STAGE *****
float PID_KP_REFLOW          = 300;
float PID_KI_REFLOW          = 0.05;
float PID_KD_REFLOW          = 350; 
#define PID_SAMPLE_TIME 1000






#endif // HWSETUP_HPP
