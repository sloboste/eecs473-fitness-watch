#include <stdbool.h>
#include <stddef.h>
#include "nrf.h"
#include "app_util_platform.h"
#include "time_keeper.h"

static rtc_time_t curr_time;

static void update_curr_time(){
    CRITICAL_REGION_ENTER();
    ++curr_time.milli;
    if (curr_time.milli == 100) {
        ++curr_time.seconds;
        curr_time.milli = 0;
    }
    if (curr_time.seconds == 60) {
        ++curr_time.minutes;
        curr_time.seconds = 0;
    }
    if (curr_time.minutes == 60) {
        ++curr_time.hours;
        curr_time.minutes = 0;
    }
    if (curr_time.hours == 24) {
        curr_time.seconds = 0;
        curr_time.minutes = 0;
        curr_time.hours = 0;
        curr_time.milli = 0;
    }
    CRITICAL_REGION_EXIT();
}

void rtc_config(void)
{
    // Enable Interrupt for the RTC in the core
    NVIC_EnableIRQ(RTC0_IRQn);
    // Set prescaler to a TICK of RTC_FREQUENCY
    NRF_RTC0->PRESCALER = COUNTER_PRESCALER;
    // Compare0 after approx COMPARE_COUNTERTIME seconds
    NRF_RTC0->CC[0] = COMPARE_COUNTERTIME * (RTC_FREQUENCY/100);

    // Enable TICK event and TICK interrupt:
    NRF_RTC0->EVTENSET = RTC_EVTENSET_TICK_Msk;
    NRF_RTC0->INTENSET = RTC_INTENSET_TICK_Msk;

    // Enable COMPARE0 event and COMPARE0 interrupt:
    NRF_RTC0->EVTENSET = RTC_EVTENSET_COMPARE0_Msk;
    NRF_RTC0->INTENSET = RTC_INTENSET_COMPARE0_Msk;

    curr_time.hours = 0;
    curr_time.minutes = 0;
    curr_time.seconds = 0;
    curr_time.milli = 0;
}

void rtc_start()
{
    NRF_RTC0->TASKS_START = 1;
}

void rtc_stop()
{
    NRF_RTC0->TASKS_STOP = 1;
}

void RTC0_IRQHandler()
{
    if ((NRF_RTC0->EVENTS_TICK != 0) &&
        ((NRF_RTC0->INTENSET & RTC_INTENSET_TICK_Msk) != 0))
    {
        NRF_RTC0->EVENTS_TICK = 0;
        //do something on a tick here
    }
    if ((NRF_RTC0->EVENTS_COMPARE[0] != 0) &&
        ((NRF_RTC0->INTENSET & RTC_INTENSET_COMPARE0_Msk) != 0))
    {
        NRF_RTC0->EVENTS_COMPARE[0] = 0;
        NRF_RTC0->TASKS_CLEAR = 1;
        // Update time on compare match
        update_curr_time();
    }
}

void rtc_set_time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t milli)
{
    CRITICAL_REGION_ENTER();
    curr_time.hours = hours;
    curr_time.minutes = minutes;
    curr_time.seconds = seconds;
    curr_time.milli = milli;
    CRITICAL_REGION_EXIT();
}

rtc_time_t rtc_get_time()
{
    rtc_time_t time;
    CRITICAL_REGION_ENTER();
    time = curr_time; 
    CRITICAL_REGION_EXIT();
    return time;
}

/*
void lfclk_config(void)
{
  NRF_CLOCK->LFCLKSRC = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
  NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
  NRF_CLOCK->TASKS_LFCLKSTART = 1;
  while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
  {
  }
  NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
}
*/
