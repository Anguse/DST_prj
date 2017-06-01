#include "Keypad.h"
#include "Display.h"
#include "system_sam3x.h" 
#include "at91sam3x8.h"
#include "Servo.h"
#include "Temp.h"
#include "Interrupt.h"
#include "Display.h"
#include "Main_Menu.h"
#include "Alarm.h"

int case_1;
int case_2;
int case_3;

void menu(void) {
	measure_temp();
	temp_delay(600);
	measure_temp();
	temp_delay(600);
	while (1) {
                refresh();
		switch (read_keypad()) {
		case 1:
			clear_display();
			case_1 = 1;
                        draw_temp();
			while (case_1 == 1) {
				refresh();
                                draw_min();
                                draw_max();
                                draw_average();
				if (read_keypad() == 12) { case_1 = 0; }

			}
			//Temperature
			clear_display();
			break;
		case 2:
			//Alarm
			clear_display();
			set_alarm();
			clear_display();
			break;
		case 3:
			//Find light source
			case_3 = 1;
			clear_display();
                        display_pointer( 5,8 );
                        string_2_display("Angle: ");
                        display_pointer(22, 15);
                        string_2_display("#:Exit");
			while (case_3 == 1) {
				track_light();
				refresh();
                                angle_2_display(get_angle());
				//measure light, check for changes
				if (read_keypad() == 12) { case_3 = 0; }
			}
			set_servo(0);
			clear_display();
			break;
		case 4:
			//Fast mode
			change_systick();
			break;
		default:
			display_pointer(11, 1);
			string_2_display("Main Menu");
			display_pointer(2, 4);
			string_2_display("1. Temperature");
			display_pointer(2, 7);
			string_2_display("2. Alarm");
			display_pointer(2, 10);
			string_2_display("3. Find lightsource");
			display_pointer(2, 13);
			string_2_display("4. Enable/Disable Fast-mode");
		}
	}
}

void refresh(){
  if(warning == 1){
                  display_pointer(29,15);
                  string_2_display("*");
                }
		if (systick >= x) {//Systick time?
			increase_time();
			if (fast_tick >= 600) {
				measure_temp();
				store_temp();
				fast_tick = 0;
			}
			systick = 0;
			time_2_display(time);
		}
}