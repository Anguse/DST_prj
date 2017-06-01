#include "at91sam3x8.h"
#include "Servo.h"
#include "Light.h"
#include "Keypad.h"
#include "Temp.h"
float r;
float l;
int angle = 0;
float light_max;

int degree = 1800;

void servo_init(void) {
	*AT91C_PWMC_ENA = (1 << 1);     //Enables PWM on CH1
	*AT91C_PIOB_PDR = (1 << 17);  //Disables PIO, enabling peripheral control
	*AT91C_PIOB_ABMR = (1 << 17);  //Activates periphial B
	*AT91C_PWMC_CH1_CMR = (*AT91C_PWMC_CH1_CMR | 0x5);
	*AT91C_PWMC_CH1_CPRDR = 52500;
	*AT91C_PWMC_CH1_CDTYR = 2625;
	*AT91C_PWMC_CH1_CDTYUPDR = degree;
}

void turn_servo(int ratio) {
	if (degree+(ratio*22)>=1800 && degree+ratio*22<=5760) {
		degree = degree + (ratio * 22);
		*AT91C_PWMC_CH1_CDTYUPDR = degree;

	}
}

void set_servo(int angle) {
  if(degree+angle*22>=1800&&degree+angle*22<=5760){
	degree = 1800+angle*22;
	*AT91C_PWMC_CH1_CDTYUPDR = degree;
  }
}

void servo_keybad(void) {
	int rvalue;
	rvalue = read_keypad();
	rvalue = rvalue * 10;
	turn_servo(rvalue);
}

int get_angle() {
	return (degree-1800)/22;
}


void track_light() {
	r = measure_lightr();
	l = measure_lightl();
	if ((r - l) > 40 || (l - r) > 40) {
		if (l > r) {
			turn_servo(-10);
		}
		if (l < r) {
			turn_servo(10);
		}
		temp_delay(60);
	}
}