	/* Header file of HX711 */



#ifndef hx711_h
#define hx711_h

#include "stdint.h"
#include "stm32f4xx_hal.h"
#include "math.h"
#include "stdbool.h"

extern int GAIN;											// User defined gain value.
extern long OFFSET;										// Offset value obtained from average reading.
extern float SCALE;										// Appropriate scale value for optimization.


void begin(uint8_t gain);							// Allows to set the gain.
bool is_ready();											// Checks whether the sensor is ready for retrieval.
void set_gain(uint8_t gain);					// Sets the gain factor.(32,64 or 128)
long read();													// Waits for the chip to be ready and returns a reading.
long read_average(uint8_t times);			// Returns an average reading.
double get_value(uint8_t times);			// Returns the tare value.
float get_units(uint8_t times);				// Returns the raw value that is divided by scale.
void tare(uint8_t times);							// Assign the average value to offset value. 
void set_scale(float scale);					// Set the scale value.
float get_scale();										// Return the current scale value.
void set_offset(long offset);					// Set the offset value that is read from the tare weight.
long get_offset();										// Return the offset value.
void power_down();										// Puts the sensor into the power down mode.
void power_up();											// Wakes up the sensor and returns it to normal operation mode.


#endif
