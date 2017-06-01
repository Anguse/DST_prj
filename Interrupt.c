#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "Interrupt.h"
#include "Temp.h"
#include "Display.h"

int buttonInterrupt = 0;
int systick = 0;
int fast_tick = 0;
int delay_1 = 0;
int ticks;
int time[6] = { 0,0,0,0,0,0 };
int x = 600;
void enableSystick(int value) {
	SysTick_Config(value);
}
void increase_time() {
	ticks++;
	time[0] = ticks % 10;
	time[1] = (ticks / 10) % 6;
	time[2] = (ticks / 60) % 10;
	time[3] = (ticks / 600) % 6;
	time[4] = (ticks / 3600) % 10;
	time[5] = (ticks / 36000) % 6;
	if (((ticks % 86400) == 0) && (ticks != 0)) {
          if(day<6){
             day++;
          }
          else{
            reset_data();
          }
		ticks = 0;
	}
}
void SysTick_Handler() {
	systick++;
	fast_tick++;
	if (delay_flag == 1) {
		delay_1++;
	}
}

void PIOD_Handler(void) {
	int button = (*AT91C_PIOD_ISR >> 0) & 1;
	int clock = *AT91C_NVIC_STICKCSR & (1);
	if (button == 0) {        //Did the button cause the interrupt?
		buttonInterrupt++;
		NVIC_ClearPendingIRQ(PIOD_IRQn);
		if (clock == 1) {     //If the systick is enabled, disable it
			*AT91C_NVIC_STICKCSR &= ~(1 << 0);
		}
		else {               //Else enable it
			enableSystick(8400000);
		}
	}
}
void change_systick() {
	if (x == 600) {
		x = 0;
	}
	else {
		x = 600;
	}
	systick = 0;
}

