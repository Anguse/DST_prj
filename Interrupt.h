#ifndef Interrupt_h
#define Interrupt_h
extern int fast_tick;
extern int systick;
extern int delay_1;
extern int x;
extern int ticks;
extern int time[6];
void change_systick();
void enableSystick(int value);
void Systick_Handler(void);
void PIOD_Handler(void);
void increase_time();
void warning_high();
void warning_low();
#endif