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

void menu(void){
  enableSystick(140000);
  init_temp();
  measure_temp();
  temp_delay(600);
  while(1){
    if(systick>=x){//Systick time?
        increase_time();
        if(systick>=600){
          measure_temp();
        }
        systick = 0;
    }     
    time_2_display(time);
    store_temp();
    temp_delay(6);
    switch(read_keypad()){
    case 1 :
      clear_display();
      case_1 = 1;
      while(case_1 == 1){
        if(systick>=x){//Systick time?
        systick = 0;
        increase_time();
        measure_temp();
        }
        
        time_2_display(time);
        store_temp();
        draw_temp();
        if(read_keypad() == 12){case_1 = 0;}
        
      }
      //Temperature
      clear_display();
      break;
    case 2 :
      //Alarm
      clear_display();
      set_alarm();
      clear_display();
      break;
    case 3 :
      //Find light source
      case_3 = 1;
      traco_light();
      while(case_3 == 1){
        if(systick>=x){//Systick time?
        systick = 0;
        increase_time();
        measure_temp();
      }
      //measure light, check for changes
       /* if(){   //Change noticed
        track_light();
        }*/
       if(read_keypad() == 12){case_3 = 0;} 
      }
      set_servo(0);
      break;
    case 4 :
      //Fast mode
      change_systick();
      break;
    default : 
      display_pointer(11,1);
      string_2_display("Main Menu");
      display_pointer(2,4);
      string_2_display("1. Temperature");
      display_pointer(2,7);
      string_2_display("2. Alarm");
      display_pointer(2,10);
      string_2_display("3. Find lightsource");
      display_pointer(2,13);
      string_2_display("4. Enable/Disable Fast-mode");
    }
  }
}