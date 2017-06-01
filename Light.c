#include "at91sam3x8.h"
#include "Interrupt.h"
#include "Light.h"
#include "core_cm3.h"


void init_light_sensor(void) {
	*AT91C_ADCC_MR = 0x200;          //Set prescaler to 2
}


float measure_lightr(void) {
	*AT91C_ADCC_CHER = 0x2;        //Select channel 1 
	*AT91C_ADCC_CR = 0x2;          //Start an ADC
	*AT91C_ADCC_IER = (1 << 24);      //Wait for DRDY
	float lightr;
	lightr = (*AT91C_ADCC_CDR1 & 0xFFF);       //Gets value from last conversion
	return lightr;
}

float measure_lightl(void) {
	*AT91C_ADCC_CHER = 0x4;        //Select channel 2
	*AT91C_ADCC_CR = 0x2;          //Start an ADC
	*AT91C_ADCC_IER = (1 << 24);    //Wait for DRDY
	float lightl;
	lightl = (*AT91C_ADCC_CDR2 & 0xFFF);           //Gets value from last conversion
	return lightl;
}