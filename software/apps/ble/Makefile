PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += ble_debug_assert_handler.c
APPLICATION_SRCS += app_util_platform.c

APPLICATION_SRCS += app_timer.c
APPLICATION_SRCS += app_scheduler.c
APPLICATION_SRCS += app_timer_appsh.c

APPLICATION_SRCS += softdevice_handler.c
APPLICATION_SRCS += ble_srv_common.c
APPLICATION_SRCS += ble_advdata.c
APPLICATION_SRCS += ble_conn_params.c
APPLICATION_SRCS += ble_config.c
APPLICATION_SRCS += watch_service.c 
APPLICATION_SRCS += packets.c 
APPLICATION_SRCS += timer_config.c 
APPLICATION_SRCS += scheduler_config.c 

APPLICATION_SRCS += nrf_delay.c 

ifndef TARGET
    TARGET = PCB
endif

TARGET_UPPER = $(shell echo $(TARGET) | tr a-z A-Z)
TARGET_LOWER = $(shell echo $(TARGET) | tr A-Z a-z)
BOARD = BOARD_$(TARGET_UPPER)

APPLICATION_SRCS += $(TARGET_LOWER).c

LIBRARY_PATHS += ./ ../../platform/
SOURCE_PATHS += ../../platform/

LIBRARY_PATHS += ../../lib/
SOURCE_PATHS += ../../lib/
LIBRARY_PATHS += ../../lib/ble/
SOURCE_PATHS += ../../lib/ble/
LIBRARY_PATHS += ../../lib/scheduler/
SOURCE_PATHS += ../../lib/scheduler/
LIBRARY_PATHS += ../../lib/timer/
SOURCE_PATHS += ../../lib/timer/

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
