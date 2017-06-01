#include <stdio.h>
#include "Display.h"
#include "system_sam3x.h" 
#include "at91sam3x8.h"
#include "Temp.h"


void delay(int value) {
	for (int i = 0; i < value; i++) {
		asm("nop");
	}
}

unsigned char read_status_display(void) {
	unsigned char temp;

	*AT91C_PIOC_SODR = 0x3FC;     //Databus as input
	*AT91C_PIOC_SODR = 0x2000;     //DIR as input
	*AT91C_PIOC_CODR = 0x1000;    //DIR as output (CLEAR)
	*AT91C_PIOC_SODR = 0x20000;   //Set C/D
	*AT91C_PIOC_CODR = 0x10000;   //Clear CE
	*AT91C_PIOC_CODR = 0x8000;    //Clear RD
	delay(50);                    //~120nS delay
	temp = (*AT91C_PIOC_PDSR >> 2);
	temp = temp & 0x3;
	*AT91C_PIOC_SODR = 0x10000;   //Set CE
	*AT91C_PIOC_SODR = 0x8000;    //Set RD
	*AT91C_PIOC_SODR = 0x1000;    //Output disabled
	*AT91C_PIOC_CODR = 0x2000;    //Clear DIR -> Output
	return temp;

}
void Write_Command_2_Display(unsigned char command) {
	unsigned char temp = 0;
	while (temp != 3) {        //Wait until display is ready
		temp = read_status_display();
	}
	*AT91C_PIOC_CODR = 0x3FC;     //Clear databus
	*AT91C_PIOC_SODR = (command << 2); //Command to databus
	*AT91C_PIOC_CODR = 0x2000;    //DIR as output
	*AT91C_PIOC_CODR = 0x1000;    //Output enable
	*AT91C_PIOC_OER = 0x3FC;     //Databus as output
	*AT91C_PIOC_SODR = 0x20000;   //Set C/D
	*AT91C_PIOC_CODR = 0x10000;   //Clear CE
	*AT91C_PIOC_CODR = 0x4000;    //Clear write display?
	delay(50);
	*AT91C_PIOC_SODR = 0x10000;   //Set chip enable display
	*AT91C_PIOC_SODR = 0x4000;    //Set write display
	*AT91C_PIOC_SODR = 0x1000;    //Output disabled
	*AT91C_PIOC_ODR = 0x3FC;     //Databus as input
}
void Write_Data_2_Display(unsigned char data) {
	unsigned char temp = 0;
	while (temp != 3) {        //Wait until display is ready
		temp = read_status_display();
	}
	*AT91C_PIOC_CODR = 0x3FC;     //Clear databus
	*AT91C_PIOC_SODR = (data << 2); //Command to databus
	*AT91C_PIOC_CODR = 0x2000;    //DIR as output
	*AT91C_PIOC_CODR = 0x1000;    //Output enable
	*AT91C_PIOC_OER = 0x3FC;     //Databus as output
	*AT91C_PIOC_CODR = 0x20000;   //Clear C/D
	*AT91C_PIOC_CODR = 0x10000;   //Clear CE
	*AT91C_PIOC_CODR = 0x4000;    //Clear write display?
	delay(50);
	*AT91C_PIOC_SODR = 0x10000;   //Set chip enable display
	*AT91C_PIOC_SODR = 0x4000;    //Set write display
	*AT91C_PIOC_SODR = 0x1000;    //Output disabled
	*AT91C_PIOC_ODR = 0x3FC;     //Databus as input
}
void clear_display(void) {
	Write_Data_2_Display(0x00);
	Write_Data_2_Display(0x00);
	Write_Command_2_Display(0x24);

	for (int a = 0; a < 1000; a++) {
		Write_Data_2_Display(0x00);
		Write_Command_2_Display(0xC0);
	}

	Write_Data_2_Display(0x00);
	Write_Data_2_Display(0x00);
	Write_Command_2_Display(0x24);
}
void init_display(void) {

	*AT91C_PIOC_PER = 0xFF3FC;
	*AT91C_PIOC_OER = 0xFF3FC;
	*AT91C_PIOD_PER = 0x1;
	*AT91C_PIOD_OER = 0x1;
	*AT91C_PIOD_CODR = 0x1; // Clear reset Display

	delay(50);

	*AT91C_PIOD_SODR = 0x1; // Set reset Display

	Write_Data_2_Display(0x00);
	Write_Data_2_Display(0x00);
	Write_Command_2_Display(0x40);//Set text home address
	Write_Data_2_Display(0x00);
	Write_Data_2_Display(0x40);
	Write_Command_2_Display(0x42); //Set graphic home address
	Write_Data_2_Display(0x1e);
	Write_Data_2_Display(0x00);
	Write_Command_2_Display(0x41); // Set text area
	Write_Data_2_Display(0x1e);
	Write_Data_2_Display(0x00);
	Write_Command_2_Display(0x43); // Set graphic area
	Write_Command_2_Display(0x80); // text mode
	Write_Command_2_Display(0x94); // Text on graphic off 

}
void int_2_display(int key) {
	if (key != 0) {
		unsigned int commands[13] = { 0x00,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0xA,0x10,0x3 };
		Write_Data_2_Display(commands[key]);
		Write_Command_2_Display(0xC0);
	}

}
void display_pointer(int x, int y) {
	int pos = x + (y * 30);

	if (pos > 255) {
		Write_Data_2_Display(pos);
		Write_Data_2_Display(1);
		Write_Command_2_Display(0x24);
	}

	else {
		Write_Data_2_Display(pos);
		Write_Data_2_Display(0);
		Write_Command_2_Display(0x24);
	}
}

void float_2_display(float temp) {
	char string[4];
	sprintf(string, "%f", temp);
	unsigned int commands[10] = { 0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19 };
	for (int i = 0; i < 4; i++) {
		if (string[i] == '.') {
			Write_Data_2_Display(0x0E);
			Write_Command_2_Display(0xC0);
		}
		else {
			Write_Data_2_Display(commands[string[i] - '0']);
			Write_Command_2_Display(0xC0);
		}
	}
	display_pointer(14, 8);

}
void angle_2_display(int temp) {
	display_pointer(12, 8);
	char string[4];
	sprintf(string, "%d", temp);
	unsigned int commands[10] = { 0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19 };
	for (int i = 0; i < 4; i++) {
		Write_Data_2_Display(commands[string[i] - '0']);
		Write_Command_2_Display(0xC0);
	}
}

void string_2_display(char *string) {
	while (*string) {
		Write_Data_2_Display(*string - 32);
		Write_Command_2_Display(0xC0);
		*string++;
	}
}

void time_2_display(int timer[]) {
	display_pointer(22, 0);
	int index = 5;
	for (int i = 0; i < 8; i++) {
		if (i == 2 || i == 5) {
			string_2_display(":");
		}
		else {
			if (timer[index] == 0) {
				int_2_display(11);
			}
			else {
				int_2_display(timer[index]);
			}
			index--;
		}
	}
	display_pointer(26, 1);
	string_2_display("DAY");
	int_2_display((day + 1));
}
