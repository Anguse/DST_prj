/*Menu for adjusting and setting the alarm*/

#include "Alarm.h"
#include "system_sam3x.h"
#include "core_cm3.h"
#include "Temp.h"
#include "Display.h"
#include "Keypad.h"
#include "Interrupt.h"
#include "Main_Menu.h"

int key;
float value_max = 25.0;
float value_min = 20.0;
int done = 0;
int enabled = 0;

void set_alarm() {
	/*Layout for the alarm menu*/
	display_pointer(22, 15);
	string_2_display("#:Exit");
	display_pointer(11, 1);
	string_2_display("Set Alarm");
	display_pointer(2, 4);
	string_2_display("1. Increase");
	display_pointer(2, 6);
	string_2_display("4. Decrease");
	display_pointer(2, 10);
	string_2_display("2. Increase");
	display_pointer(2, 12);
	string_2_display("5. Decrease");
	display_pointer(17, 5);
	string_2_display("Max: ");
	display_pointer(17, 11);
	string_2_display("Min: ");
	display_pointer(2, 15);
	string_2_display("*:");

	/*Read user input until '*' is pressed*/
	done = 0;
	while (done == 0) {
		temp_delay(75);
		display_pointer(22,5);
		float_2_display(value_max);
		display_pointer(22,11);
		float_2_display(value_min);

		if (enabled == 0) {
			display_pointer(4, 15);
			string_2_display("       ");
			display_pointer(4, 15);
			string_2_display("Enable");
		}
		else {
			display_pointer(4, 15);
			string_2_display("Disable");
		}

		refresh();

		switch (read_keypad()) {

		case 1:
			if (value_max < 50) {
				value_max += 0.5;
			}
			break;

		case 4:
			if (value_max > value_min) {
				value_max -= 0.5;
			}
			break;
		case 2:
			if (value_min < value_max) {
				value_min += 0.5;
			}
			break;
		case 5:
			if (value_min > 0) {
				value_min -= 0.5;
			}
			break;
		case 10:
                        warning_performed = 0;
			if (enabled == 0) {
				enabled = 1;
			}
			else {
                                warning = 0;
				enabled = 0;
			}
			break;
		case 12:
			done = 1;
			break;
		default:

			break;
		}

	}

}

