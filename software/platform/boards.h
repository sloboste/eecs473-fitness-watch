#ifndef BOARDS_H
#define BOARDS_H

#include "nrf51.h"

#ifdef BLUE_DEV_BOARD 

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

    // SW I2C / Fuel gauge
    #define PIN_SW_I2C_SDA  1
    #define PIN_SW_I2C_SCL  2
    #define PIN_FUEL_QST    3

    // HW I2C
    #define TWI_MASTER_CONFIG_CLOCK_PIN_NUMBER (7U)
    #define TWI_MASTER_CONFIG_DATA_PIN_NUMBER (30U)

#else 
    #ifdef PCB

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
        #define PIN_RTS 2 
        #define PIN_TXD 12
        #define PIN_CTS 3
        #define PIN_RXD 11

        // SPI
        #define NRF_SPI NRF_SPI0
        #define SPI_SCK_PIN  5                                                          
        #define SPI_MOSI_PIN 6                                                         
        #define SPI_MISO_PIN 0 // Not connected                                                         
        #define SPI_SS_PIN   7 

        // SW I2C / Fuel gauge
        #define PIN_SW_I2C_SDA  13
        #define PIN_SW_I2C_SCL  14
        #define PIN_FUEL_QST    15

        // HW I2C
        #define TWI_MASTER_CONFIG_CLOCK_PIN_NUMBER (25U)
        #define TWI_MASTER_CONFIG_DATA_PIN_NUMBER (24U)

    #else
        #error You must define a BOARD_NAME in Makefile
    #endif
#endif

#endif  // BOARDS_H
