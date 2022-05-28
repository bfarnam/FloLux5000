# FloLux5000
### A Brett-er way to reflow - cause I can't leave well enough alone!

I decided to convert an old 1500w Cuisinart convection oven into a proper reflow oven.  I have been using it in "manual" mode with two thermocouples for several years now and decided to take the plunge.  

## 05/28/2022 - WORK IN PROGRESS

### Requirements
Independent control of Top and Bottom Elements - using i2c  
Independent control of "Kicker" Heat if Needed - using i2c  
Independent control of Convection Fan - using i2c  
Touchscreen Interface - Either 4 pin or SPI  
Minimum 2.8" TFT LCD - Either 8 Bit Parallel or SPI  
Graphing Output on Display  
Compatible with MC9600 i2c Thermocouple Module  
Independent "Small" i2c Display to show "actual" temperature in digits  
Independent "Small" i2c Display to show "set point" temperature in digits  
Optional door opener  
Optional Single Rotary Button Encoder  

### Zero Cross vs Phase Angle vs Solid State Voltage Regulator (SSVR) vs Digital Solid State Power Regulator... uh what?
There are many different ways of controlling AC output and I will explain briefly here the 20,000 foot overview.  Auber Instruments has a LOT of info on thier website on this.  
Source: https://www.auberins.com/images/Manual/EZboil_talkv1.1.pdf

#### Zero Cross
The most common method of turning On and Off a high voltage AC load is using just a plain old Zero Cross SSR.  The SSR will only turn on or off when the AC sign wave crosses the 0v line - hence Zero Cross.  I won't get into the specifics, but many Commercial Off The Shelf (COTS) PID units when utilizing a Zero Cross SSR cycle on and off in 2 second intervals using a technique called Burst Mode.

##### Pros
Used Often - Many Amazon SSR's are Zero-Cross  
A Common Method of Control with Many Reflow Code Examples  
Does Not Require an Interrupt and is compatible with i2c  
Very Low EMI and RFI  
Prolongs the life of the load and SSR  

##### Cons
An improperly tuned PID setup will overshoot, undershoot, and have a hard time maintaining a temperature set point over a period of time.
Very hard to manage fine resolution (control) due to the fact that you can't control the exact turn on or turn off time - usually (more on that later).
Not suitable for certain type of loads.  

#### Phase Angle
Another method of control that is becoming more and more common and seems to be used a lot in many different reflow examples.  When used with an SSR, the terms Phase Angle and Random Fire are used interchangeably.  When used with a dedicated Triac, the term Phase Angle, Phase Control, and PWM are used interchangeably.  In the simplest terms, when the MCU is notified via interrupt that a zero cross event has occurred, a timer kicks off relative to the percent of on/off time required, then sends a pulse to turn on the load.  The load will automatically turn off when the next zero cross event happens and the process starts all over.  Another common method of control that many Commercial Off The Shelf (COTS) PID units use when utilizing a Random Fire SSR to cycle on and off in less than 2 second intervals using a technique called Time Proportional Firing.  


##### Pros
Very Very fine control of the output percentage which translates to control down to the degree  
Many Reflow Code Examples  
Tried and True Method  

##### Cons
Requires an Interrupt and Timer setup and therefore not compatible with i2c communications  
Can *and usually* causes EMI or RFI with certain loads  
Can be hard on some loads  
Can shorten the life of an SSR (due to switch on near peaks of the voltage potential)  
Need to know how many cycles per millisecond in order to determine the timer length.  A micro delay can be used instead of calculating the cycles per ms.

#### Solid State Voltage Regulator (SSVR)
Another method of control very similar to Phase Angle control where the system controls a Triac via an analog input to calculate the Phase Angle and can very precisely control the output AC Voltage.  Used in many light switch dimmer circuits.  

##### Pros
Very Very fine control of the output voltage which translates to control down to the degree  
Proven - Used a LOT among the home brewers, distillers, and cannabis extractors  
Because the load control takes an analog input, does not require an Interrupt or Timer  

##### Cons
Hard to integrate into the standard PID libraries (but is PID even needed)  
Does not work with certain loads  
Not many reflow ovens use this method  

#### Digital Solid State Power Regulator... uh what?
Yet another method of control which kind of blends the Zero Cross with the Phase Angle.  This is time based and instead of firing the Zero Cross in 2 second intervals (Burst Mode), the system fires the SSR in half wave pulses.  At 60 Hz (here in the US) that amounts to on pulses at a minimum of 8.35 milliseconds (a full period is 16.67 milliseconds).  This is usually averaged over a period of 100 cycles to give you a percentage of on/off time.  This is called Time Proportional Firing.

##### Pros
Yet again fine control of the output which translates to control down to the degree  
Becoming very popular among ethanol distillers as it allows usage of kicker elements to heat up very quickly and then keep a set point for a period of time  
Does not require any PID tuning  

##### Cons
Requires an Interrupt to detect Zero Cross and therefor is not compatible with i2c.
Requires that the "phase" of the AC voltage be known.
Requires the "mass" of the load and the "resistance" of the load to temperature change in order to calculate the rate of temperature change based upon the wattage of the heating elements  
Could not find any reflow examples, probably because the mass and resistance are usually unknown  

