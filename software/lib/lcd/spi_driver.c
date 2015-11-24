#include <stdint.h>

#include "nrf_gpio.h"
#include "spi_driver.h"

//Commands
#define WRITE_REG 0x0A
#define READ_REG 0x0B

void spi_init(){
    // initialize spi
    nrf_gpio_cfg_output(SPI_SS_PIN);
    nrf_gpio_pin_clear(SPI_SS_PIN);
    NRF_SPI->PSELSCK    =   SPI_SCK_PIN;
    NRF_SPI->PSELMOSI   =   SPI_MOSI_PIN;
    NRF_SPI->PSELMISO   =   SPI_MISO_PIN;
    NRF_SPI->FREQUENCY  =   SPI_FREQUENCY_FREQUENCY_K500;
    NRF_SPI->CONFIG =   (uint32_t)(SPI_CONFIG_CPHA_Leading << SPI_CONFIG_CPHA_Pos) |
                        (SPI_CONFIG_CPOL_ActiveHigh << SPI_CONFIG_CPOL_Pos) |
                        (SPI_CONFIG_ORDER_MsbFirst << SPI_CONFIG_ORDER_Pos);
    NRF_SPI->ENABLE = (SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos);
    //NRF_SPI0->ENABLE = 0;
    NRF_SPI->EVENTS_READY = 0;

}

void spi_write(uint8_t buf) {
    //clear the ready event
    //NRF_SPI0->ENABLE = 1;

    NRF_SPI->EVENTS_READY = 0;

    NRF_SPI->TXD = buf;

    //wait until byte has transmitted
    while(NRF_SPI->EVENTS_READY == 0);

    uint8_t throw = NRF_SPI->RXD;

    NRF_SPI->EVENTS_READY = 0;

    //NRF_SPI0->ENABLE = 0;

}

void spi_read(uint8_t* buf) {

    //NRF_SPI0->ENABLE = 1;

    //clear ready event
    NRF_SPI->EVENTS_READY = 0;

    NRF_SPI->TXD = 0x00;

    //wait until byte has been received
    while(NRF_SPI->EVENTS_READY == 0);

    buf[0] = NRF_SPI->RXD;

    NRF_SPI->EVENTS_READY = 0;

    //NRF_SPI0->ENABLE = 0;

}

void spi_write_reg(uint8_t reg_addr, uint8_t * data, uint8_t num_bytes){

    //NRF_SPI0->ENABLE = 1;


    nrf_gpio_pin_clear(SPI_SS_PIN);
    spi_write(WRITE_REG);
    spi_write(reg_addr);

    int i = 0;
    do{
        spi_write(data[i]);
        i++;
    } while (i < num_bytes);

    nrf_gpio_pin_set(SPI_SS_PIN);

    //NRF_SPI0->ENABLE = 0;


}

void spi_read_reg(uint8_t reg_addr, uint8_t * data, uint8_t num_bytes){

    //NRF_SPI0->ENABLE = 1;


    nrf_gpio_pin_clear(SPI_SS_PIN);
    spi_write(READ_REG);
    spi_write(reg_addr);

    int i = 0;
    do{
        spi_read(data + i);
        i++;
    } while (i < num_bytes);

    nrf_gpio_pin_set(SPI_SS_PIN);

    //NRF_SPI0->ENABLE = 0;


}


void spi_disable(){

    NRF_SPI->ENABLE = (SPI_ENABLE_ENABLE_Disabled << SPI_ENABLE_ENABLE_Pos);

}

void spi_enable(){

    NRF_SPI->ENABLE = (SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos);

}
