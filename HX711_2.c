	/* Source file of HX711 */
	  
	
	
	#include "HX711_2.h"

	int 	GAIN;					// The user defined gain value.
	long 	OFFSET	= 0;	// The offset value used for tare weight.
	float SCALE 	= 1;	// The scale value in order to get more accurate value.

	void begin(uint8_t gain){
		
		// The user is able to set gain.
		set_gain(gain);

	
}

	bool is_ready(){
		
		// When the data line goes low, the sensor is ready for retrieval.
		return HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13)==0;
	
		
}

	void set_gain(uint8_t gain){

		switch (gain) {
			case 128:		// channel A, gain factor 128
				GAIN = 1;
				break;
			case 64:		// channel A, gain factor 64
				GAIN = 3;
				break;
			case 32:		// channel B, gain factor 32
				GAIN = 2;
				break;
	}
	
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);
		read();
	
}

	long read(){

		// It checks the start sequence condition.
		while(!is_ready());
	
		unsigned long value = 0;
		// 3 array each of 8 bit contain 24 bit data.
		uint8_t data[3] = {0};
		// The filler is used for preventing conflict if any in the case of negative number representation.
		uint8_t filler = 0x00;
	
		for(int i = 0; i<8 ; i++){
			
			// The clock line is set.
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
			// The obtaining data is shifted(MSB first).
			data[2] |= HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) << (7-i);
			// The clock line is reset.
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);
	
	}

		for(int i = 0; i<8 ; i++){
			
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
	

			data[1] |= HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) << (7-i);
			
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);
	
		}

		for(int i = 0; i<8 ; i++){

			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
	

			data[0] |= HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) << (7-i);
		

			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);
	
		}

		// Channel and the gain factor are set for the next reading.
		for(unsigned int i=0 ; i<GAIN; i++){

			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);
		}
	
		// It replicates the most significant bit to pad out a 32-bit signed integer.
		if (data[2] & 0x80) {
			filler = 0xFF;
		} else {
			filler = 0x00;
		}
		// It constructs a 32-bit signed integer.
		value = ( (unsigned long)(filler) << 24
				| (unsigned long)(data[2]) << 16
				| (unsigned long)(data[1]) << 8
				| (unsigned long)(data[0]) );

		return (long)(value);
	
	}
	
	long read_average(uint8_t times){

		long sum = 0;
		for(int i =0; i<times ; i++){
			sum += read();
			//It does nothing but wait.
			HAL_Delay(10); 
		}
		return sum / times;
	}

	double get_value(uint8_t times){
		
		// It provides optimization.
		return read_average(times) - OFFSET;
	
	}

	float get_units(uint8_t times){

		
		return get_value(times) /( SCALE * 10);
		
	
	}

	void tare(uint8_t times){

		double sum = read_average(times);
		// The average value is assigned as offset.
		set_offset(sum);
	}

	void set_scale(float scale){

		SCALE = scale;
	
	}

	float get_scale(){
	
		return SCALE;
	}

	void set_offset(long offset){

		OFFSET = offset;
	
	}

	long get_offset(){
		
		return OFFSET;
	
	}

	void power_down(){
	
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
	}

	void power_up(){
	
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);
	}