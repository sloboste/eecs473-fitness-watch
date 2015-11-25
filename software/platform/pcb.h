/* Pin assignemnts on the custom PCB.
 * 
 * TODO fix the pin assignments
 */

#ifndef PCB_H
#define PCB_H

// Push button switches
#define PIN_BUTTON_1 23
#define PIN_BUTTON_2 22 
#define PIN_BUTTON_3 21

#define BUTTON_PULL GPIO_PIN_CNF_PULL_Pullup

// LEDs
#define PIN_LED_1 8
#define PIN_LED_2 9
#define PIN_LED_3 10

// UART
//#define PIN_RTS ? 
//#define PIN_TXD ?
//#define PIN_CTS ?
//#define PIN_RXD ?

// SPI
#define SPI_SCK_PIN  5                                                          
#define SPI_MISO_PIN 6                                                         
#define SPI_MOSI_PIN 0 // Not connected                                                         
#define SPI_SS_PIN   7 
#define NRF_SPI NRF_SPI0 // TODO should we have an include for this?

// I2C
// TODO

#endif
