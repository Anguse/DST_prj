#ifndef Temp_h
#define Temp_h
extern float thao;
extern float temperature;
extern int day;
extern int delay_flag;
extern int warning;
extern int warning_performed;
struct day_temp;
void reset_data();
void temp_delay(int value);
void measure_temp();
void init_temp();
void TC0_Handler();
void store_temp();
void draw_temp();
void draw_min();
void draw_max();
void draw_average();
#endif