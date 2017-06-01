#include "Button_LED.h"
#include "system_sam3x.h" 
#include "at91sam3x8.h"
void initiate_button_input() {
	*AT91C_PIOD_PER = 0xA;                 //PIOD_PER
	*AT91C_PIOD_ODR = 0x2;               //PIOD_ODR
	*AT91C_PIOD_PPUER = 0x2;               //PIOD_PUER
	*AT91C_PIOD_OER = 0x8;                //PIOD_OER
	*AT91C_PIOD_CODR = 0;
}
void initiate_interrupt_handler() {
	*AT91C_PIOD_AIMER = 0x2;
	*AT91C_PIOD_IFER = 0x2;
	*AT91C_PIOD_IFSR = 0x2;
	*AT91C_PIOD_SCDR = 261;
	NVIC_ClearPendingIRQ(PIOD_IRQn);
	NVIC_SetPriority(PIOD_IRQn, PIOD_IRQn);
	NVIC_EnableIRQ(PIOD_IRQn);
	*AT91C_PIOD_PER = 0x2;
	*AT91C_PIOD_IER = 0x2;
}
int readButton() {               //Checks if the button is pressed
	int button = *AT91C_PIOD_PDSR & (0x2);          //PIO_PDSR(PIOD)
	if (button == 0) {                 //Button pushed -> true
		setLED(1);
		return 1;
	}
	else {
		setLED(0);
		return 0;                     //Button not pushed -> false
	}
}

void setLED(int nLED) {          //Enables or disables LED


	if (nLED == 1) {
		*AT91C_PIOD_SODR |= (1 << 3);
	}
	else {
		*AT91C_PIOD_CODR |= (1 << 3);
	}
}
