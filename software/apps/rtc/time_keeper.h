typedef struct current_time_struct{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t milli;
} current_time_t;

current_time_t curr_time;


#define RTC_FREQUENCY             (800UL)                     /*!< required RTC working clock RTC_FREQUENCY Hertz. Changable */
#define COMPARE_COUNTERTIME       (1UL)                     /*!< Get Compare event COMPARE_TIME seconds after the counter starts from 0 */
#define LFCLK_FREQUENCY           (32768UL)

#define COUNTER_PRESCALER         ((LFCLK_FREQUENCY/RTC_FREQUENCY) - 1)  /* f = LFCLK/(prescaler + 1) */

extern void rtc_config(void);

extern void RTC0_IRQHandler();

extern void set_time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t milli);

extern current_time_t get_time();

void update_curr_time();
