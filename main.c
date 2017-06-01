
#include "system_sam3x.h" 
#include "at91sam3x8.h" 
#include "Button_LED.h"
#include "Display.h"
#include "Keypad.h"
#include "Temp.h"
#include "Interrupt.h"
#include "Light.h"
#include "Servo.h"
#include "Main_Menu.h"


void main(void) {
	SystemInit(); //Disables the watchdog and setup the MCK
	*AT91C_PMC_PCER = 0x8007800;//Peripheral Clock (PIOB,PIOC,PIOD,TC0)
	*AT91C_PMC_PCER1 = 0x30;
	enableSystick(140000);
	initiate_button_input();
	initiate_interrupt_handler();
	initiate_keypad();
	init_display();
	clear_display();
	init_light_sensor();
	servo_init();
        init_temp();
	display_pointer(0, 0);
	menu();
} // main() 