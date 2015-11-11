PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += softdevice_handler.c
APPLICATION_SRCS += ble_advdata.c
APPLICATION_SRCS += ble_conn_params.c
APPLICATION_SRCS += ble_debug_assert_handler.c
APPLICATION_SRCS += ble_srv_common.c
APPLICATION_SRCS += app_timer.c
APPLICATION_SRCS += app_util_platform.c
APPLICATION_SRCS += ble_hrs.c
APPLICATION_SRCS += ble_bas.c

#APPLICATION_SRCS += nrf_drv_twi.c
#APPLICATION_SRCS += app_scheduler.c
#APPLICATION_SRCS += app_timer_appsh.c
APPLICATION_SRCS += nrf_delay.c

APPLICATION_SRCS += app_uart.c
APPLICATION_SRCS += nrf_drv_gpiote.c
APPLICATION_SRCS += nrf_drv_common.c

APPLICATION_SRCS += battery_service.c
APPLICATION_SRCS += ble_config.c
APPLICATION_SRCS += gps_service.c 
APPLICATION_SRCS += heart_rate_service.c 
APPLICATION_SRCS += ped_service.c 

APPLICATION_SRCS += timer_config.c 


ifndef TARGET
    TARGET = GREEN_DEV_BOARD
endif

TARGET_UPPER = $(shell echo $(TARGET) | tr a-z A-Z)
TARGET_LOWER = $(shell echo $(TARGET) | tr A-Z a-z)
BOARD = BOARD_$(TARGET_UPPER)

APPLICATION_SRCS += $(TARGET_LOWER).c

LIBRARY_PATHS += ./ ../../platform/
SOURCE_PATHS += ../../platform/

LIBRARY_PATHS += ../../lib/ble/
SOURCE_PATHS += ../../lib/ble/
#LIBRARY_PATHS +=  ../../lib/gps/
#SOURCE_PATHS += ../../lib/gps/
#LIBRARY_PATHS += ../../lib/hrm/
#SOURCE_PATHS += ../../lib/hrm/
#LIBRARY_PATHS += ../../lib/i2c/
#SOURCE_PATHS += ../../lib/i2c/
#LIBRARY_PATHS += ../../lib/imu/
#SOURCE_PATHS += ../../lib/imu/
#LIBRARY_PATHS += ../../lib/lcd/
#SOURCE_PATHS += ../../lib/lcd/
#LIBRARY_PATHS += ../../lib/spi/
#SOURCE_PATHS += ../../lib/spi/
LIBRARY_PATHS += ../../lib/timer/
SOURCE_PATHS += ../../lib/timer/
#LIBRARY_PATHS += ../../lib/uart/
#SOURCE_PATHS += ../../lib/uart/

NRF_BASE_PATH ?= ../../nrf5x-base/
TEMPLATE_PATH ?= $(NRF_BASE_PATH)/nrf51-pure-gcc-setup/template/

DEVICE = NRF51
USE_SOFTDEVICE = s110
SOFTDEVICE ?= $(NRF_BASE_PATH)/softdevice/s110_nrf51_8.0.0_softdevice.hex

SDK_VERSION = 9
SDK_PATH ?= $(NRF_BASE_PATH)/sdk/nrf51_sdk_9.0.0/

CFLAGS = -Os
GDB_PORT_NUMBER = 2331

include $(NRF_BASE_PATH)/Makefile.include
include $(TEMPLATE_PATH)/Makefile