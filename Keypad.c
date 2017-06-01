#include "Keypad.h"
#include "system_sam3x.h" 
#include "at91sam3x8.h" 
#include "display.h"

void initiate_keypad() {
	*AT91C_PIOD_PER |= 1 << 2;    //Enable PIOD (pin 2)
	*AT91C_PIOC_PER = 0x3BC;      //Enable PIOC (pin 2-5,7-9)
	*AT91C_PIOC_PPUDR = 0x3C;     //Pull up disable PIOC (pin 2-5)
	*AT91C_PIOC_ODR = 0x3C;       //Output disable PIOC (pin 2-5)
	*AT91C_PIOD_OER |= 1 << 2;    //Output enable PIOD (pin 2)
	*AT91C_PIOC_OER = 0x380;      //Output enable PIOC (pin 7-9)
	*AT91C_PIOD_CODR = 0x4;       //Clear PIOD (pin 2)
	*AT91C_PIOC_CODR = 0x380;     //Clear PIOC (pin 7-9)
	*AT91C_PIOC_SODR = 0x380;     //Set PIOC (pin 7-9)
}
int read_keypad(void) {
	int value = 0;
	unsigned int row;
	*AT91C_PIOD_CODR = 0x4;       //Clear OE key bus
	*AT91C_PIOC_OER = 0x380;      //Make all column pins as output
	*AT91C_PIOC_SODR = 0x380;     //Set all column pins as high
	unsigned int colVec[3] = { 0x200,0x100,0x080 };
	for (int i = 0; i < 3; i++) {   //Columns
		*AT91C_PIOC_CODR = colVec[i];    //Clear current column
		for (int j = 0; j < 4; j++) {         //Rows
			row = *AT91C_PIOC_PDSR >> (j + 2);      //Read row
			row = row & 0x1;
			if (row == 0) {                     //check if bit is 0
				value = j * 3 + i + 1;
			}
		}

		*AT91C_PIOC_SODR = 0x380;
	}
        
	*AT91C_PIOC_ODR = 0x380;              //Make all column pin as input
	*AT91C_PIOD_SODR = 0x4;               //Set OE KEY BUS (Active Low)
	return value;
}
