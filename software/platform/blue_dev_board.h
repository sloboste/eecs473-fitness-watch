#ifndef BLUE_DEV_BOARD_H
#define BLUE_DEV_BOARD_H

#include "nrf_gpio.h"

// Push button switches
#define PIN_BUTTON_0 17
#define PIN_BUTTON_1 18
#define PIN_BUTTON_2 19
#define PIN_BUTTON_3 20

// LEDs
#define PIN_LED_1 21
#define PIN_LED_2 22 
#define PIN_LED_3 23 
#define PIN_LED_4 24

// UART
#define RX_PIN_NUMBER 11
#define TX_PIN_NUMBER 9
//#define RTS_PIN_NUMBER 0 // Doesn't matter, not used 
//#define CTS_PIN_NUMBER 0 // Doesn't matter, not used

//#define BUTTON_PULL GPIO_PIN_CNF_PULL_Pullup

#endif
