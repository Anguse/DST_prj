#include "at91sam3x8.h"
#include "Interrupt.h"
#include "Temp.h"
#include "core_cm3.h"
#include "Display.h"
#include "Alarm.h"
#include "Keypad.h"

int warning = 0;
int warning_performed = 0;

int handler_flag;

int delay_flag = 0;
struct day_temp week[7];
int day = 0;
float thao, temperature;


void init_temp(void) {
	*AT91C_TC0_CMR = 0; //Select clock as timer_clock1
	*AT91C_TC0_CCR = 1; // enables the counter
	*AT91C_TC0_CCR = 4; // making sw_reset
	temp_delay(320);
	*AT91C_TC0_CMR = (*AT91C_TC0_CMR | 0x20000);  //Load counter A when TIOA Falling
	*AT91C_TC0_CMR = (*AT91C_TC0_CMR | 0x40000);  //Load counter B when TIOA Rising 
	*AT91C_PIOB_PER = (1 << 25);     //Enable PIOB pin 25
	*AT91C_PIOB_OER = (1 << 25);     //Output
	*AT91C_PIOB_SODR = (1 << 25);     //Set bit 25
	NVIC_SetPriority(TC0_IRQn, 0);
	NVIC_EnableIRQ(TC0_IRQn);
	NVIC_ClearPendingIRQ(TC0_IRQn); //Interrupts for TC0
	*AT91C_PIOB_SODR = (1 << 25);
	delay(840);

}

struct day_temp {
	float max;
	float min;
	float average;
	int measures;
	float total;
};

void reset_data(){
  for(int i = 0; i < 7; i++){
    week[i].max = 0;
    week[i].min = 0;
    week[i].average = 0;
    week[i].measures = 0;
    week[i].total = 0;
  }
  day = 0;
}

void measure_temp(void) {
	temp_delay(6);
	*AT91C_TC0_IER = (*AT91C_TC0_IER | 0x00000040); // Interrupt enable 
	*AT91C_PIOB_OER = (1 << 25);
	*AT91C_PIOB_CODR = (1 << 25);  //start pulse
	delay(25);
	*AT91C_PIOB_SODR = (1 << 25);
	*AT91C_PIOB_ODR = (1 << 25);
	*AT91C_TC0_CCR = (*AT91C_TC0_CCR | 0x4); // making sw_reset
	temperature = ((thao / 210) - 273.15);
}

void temp_delay(int value) {
	delay_flag = 1;
	while (delay_1 < value) {}
	delay_flag = 0;
	delay_1 = 0;
}

void TC0_Handler() {
	*AT91C_TC0_IDR = (1 << 6);      //Disable interrupts for TC0
	float A = (float)(*AT91C_TC0_RA);
	float B = (float)(*AT91C_TC0_RB);
	thao = (B - A);

}



void store_temp() {
	if (week[day].measures == 0) {
		week[day].max = 0;
		week[day].min = 500;
	}
        /*Within boundarys of the alarm? (if enabled)*/
	if (enabled == 1) {

		if (temperature < value_min && warning_performed == 0) {
                        warning = 1;
                        warning_performed = 1;
			clear_display();
			display_pointer(12, 8);
			string_2_display("WARNING!!");
			display_pointer(12, 10);
			string_2_display("too cold");
                        display_pointer(21, 15);
                        string_2_display("#:Proceed");
                        while(warning == 1){
                          if(read_keypad()==12){
                            clear_display();
                            break;
                          }
                        }
		}
		else if (temperature > value_max && warning_performed == 0) {
			warning = 1;
                        warning_performed = 1;
			clear_display();
			display_pointer(12, 8);
			string_2_display("WARNING!!");
			display_pointer(12, 10);
			string_2_display("too hot");
                        display_pointer(21, 15);
                        string_2_display("#:Proceed");
                        while(warning == 1){
                          if(read_keypad()==12){
                            clear_display();
                            break;
                          }
                        }
		}
                else if(warning_performed == 1 && temperature<value_max && temperature>value_min){
                  warning = 0;
                  warning_performed = 0;
                }
                

	}

	if (temperature > week[day].max) {
		week[day].max = temperature;
	}
	if (temperature < week[day].min) {
		week[day].min = temperature;
	}
	week[day].total += temperature;
	week[day].measures++;
	week[day].average = week[day].total / week[day].measures;
}

void draw_temp() {
	char *values[4] = {"DAY","MIN","MAX","AVG"};
	char *week[7] = {"DAY 1","DAY 2","DAY 3","DAY 4","DAY 5","DAY 6","DAY 7"};
	for (int i = 0; i < 7; i++) {
		display_pointer(0, ((i + 1) * 2));
		string_2_display(week[i]);
	}
	for (int i = 0; i < 4; i++) {
		display_pointer((1 + (5 * i)), 0);
		string_2_display(values[i]);
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 21; j++) {
			display_pointer(j, (1 + (2 * i)));
			string_2_display("_");
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 17; j++) {
			display_pointer(((5 + i) + (4 * i)), j);
			string_2_display("|");
		}
	}
	display_pointer(22, 15);
	string_2_display("#:Exit");

}

void draw_min() {
	int index = 2;
	for (int i = 0; i < 7; i++) {
		display_pointer(6, index);
		float_2_display(week[i].min);
		index = index + 2;
	}
}
void draw_max() {
	int index = 2;
	for (int i = 0; i < 7; i++) {
		display_pointer(11, index);
		float_2_display(week[i].max);
		index = index + 2;
	}
}
void draw_average() {
	int index = 2;
	for (int i = 0; i < 7; i++) {
		display_pointer(16, index);
		float_2_display(week[i].average);
		index = index + 2;
	}
}
