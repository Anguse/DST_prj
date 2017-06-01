#ifndef Display_h
#define Display_h
void string_2_display(char *string);
void display_pointer(int x, int y);
void float_2_display(float temp);
void delay(int value);
unsigned char read_status_display(void);
void Write_Command_2_Display(unsigned char command);
void Write_Data_2_Display(unsigned char data);
void clear_display(void);
void init_display(void);
void int_2_display(int key);
void time_2_display(int timer[]);
void angle_2_display(int temp);
void char_2_display(char izard);
#endif