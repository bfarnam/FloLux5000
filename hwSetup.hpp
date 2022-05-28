#ifndef HWSETUP_HPP
#define HWSETUP_HPP


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

#define MS_PER_SINE  83.333                           // for 60Hz US = 83.3333ms per sinusoid
//#define MS_PER_SINE 100                             // for 50Hz = 100ms per sinusoid



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
