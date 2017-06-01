#ifndef Servo_h
#define Servo_h
extern float r;
extern float l;
void servo_init(void);
void turn_servo();
void track_light();
void set_servo();
int get_angle();
#endif