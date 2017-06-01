#ifndef Button_LED_h
#define Button_LED_h

void initiate_button_input(void);
void initiate_interrupt_handler();
int readButton(void);

void setLED(int nLED);

#endif