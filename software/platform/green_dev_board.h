#ifndef GREEN_DEV_BOARD_H
#define GREEN_DEV_BOARD_H

#include "nrf_gpio.h"

// Push button switches
#define PIN_BUTTON_0 16
#define PIN_BUTTON_1 17
#define BUTTON_PULL GPIO_PIN_CNF_PULL_Pullup

// LEDs
#define PIN_LED_0 18
#define PIN_LED_1 19

// UART
#define PIN_RTS 8
#define PIN_TXD 9
#define PIN_CTS 10
#define PIN_RXD 11

#endif
