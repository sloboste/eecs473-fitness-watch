PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += ble_debug_assert_handler.c
APPLICATION_SRCS += app_util_platform.c


APPLICATION_SRCS += nrf_delay.c
APPLICATION_SRCS += app_gpiote.c
APPLICATION_SRCS += nrf_drv_gpiote.c


ifndef TARGET
    TARGET = BLUE_DEV_BOARD
endif

TARGET_UPPER = $(shell echo $(TARGET) | tr a-z A-Z)
TARGET_LOWER = $(shell echo $(TARGET) | tr A-Z a-z)
BOARD = BOARD_$(TARGET_UPPER)

APPLICATION_SRCS += $(TARGET_LOWER).c

LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/twi_master/incubated
SOURCE_PATHS += $(SDK_PATH)/components/drivers_nrf/twi_master/incubated

LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/twi_master/incubated/config
SOURCE_PATHS += $(SDK_PATH)/components/drivers_nrf/twi_master/incubated/config

LIBRARY_PATHS += ./ ../../platform/
SOURCE_PATHS += ../../platform/

#LIBRARY_PATHS += ./ ../../platform/RTT/
#SOURCE_PATHS += ../../platform/RTT/

LIBRARY_PATHS += ../../lib/
SOURCE_PATHS += ../../lib/
LIBRARY_PATHS += ../../lib/ble/
SOURCE_PATHS += ../../lib/ble/
LIBRARY_PATHS +=  ../../lib/gps/
SOURCE_PATHS += ../../lib/gps/
#LIBRARY_PATHS += ../../lib/hrm/
#SOURCE_PATHS += ../../lib/hrm/
#LIBRARY_PATHS += ../../lib/i2c/
#SOURCE_PATHS += ../../lib/i2c/
LIBRARY_PATHS += ../../lib/imu/
SOURCE_PATHS += ../../lib/imu/
LIBRARY_PATHS += ../../lib/lcd/
SOURCE_PATHS += ../../lib/lcd/
LIBRARY_PATHS += ../../lib/scheduler/
SOURCE_PATHS += ../../lib/scheduler/
#LIBRARY_PATHS += ../../lib/spi/
#SOURCE_PATHS += ../../lib/spi/
LIBRARY_PATHS += ../../lib/timer/
SOURCE_PATHS += ../../lib/timer/
LIBRARY_PATHS += ../../lib/uart/
SOURCE_PATHS += ../../lib/uart/

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
