#ifndef BLUE_DEV_BOARD_H
#define BLUE_DEV_BOARD_H

// Push button switches
#define PIN_BUTTON_1 17
#define PIN_BUTTON_2 18
#define PIN_BUTTON_3 19
#define PIN_BUTTON_4 20

#define BUTTON_PULL GPIO_PIN_CNF_PULL_Pullup

// LEDs
#define PIN_LED_1 21
#define PIN_LED_2 22 
#define PIN_LED_3 23 
#define PIN_LED_4 24

// UART
#define PIN_RTS 8 
#define PIN_TXD 9
#define PIN_CTS 10
#define PIN_RXD 11

// SPI
#define SPI_SCK_PIN 29                                                          
#define SPI_MISO_PIN 28                                                         
#define SPI_MOSI_PIN 25                                                         
#define SPI_SS_PIN 20 
#define NRF_SPI NRF_SPI0 // TODO should we have an include for this?

// SW I2C
#define PIN_SW_I2C_SDA  1
#define PIN_SW_I2C_SCL  2

// HW I2C
// TODO
#endif
