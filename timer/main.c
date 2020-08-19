#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "bsp.h"

#include "nrf_drv_timer.h"
#include "bsp.h"
#include "app_error.h"

const nrf_drv_timer_t TIMER_LED = NRF_DRV_TIMER_INSTANCE(0);

const uint8_t leds_list[LEDS_NUMBER] = LEDS_LIST;

/*Handler for timer events.*/
void timer_led_event_handler(nrf_timer_event_t event_type, void* p_context)
{
    static uint32_t i;
    uint32_t led_to_invert = (1 << leds_list[(i++) % LEDS_NUMBER]);
    
    switch(event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
            LEDS_INVERT(led_to_invert);
            break;
        
        default:
            //Do nothing.
            break;
    }    
}


/*Function for main application entry*/
int main(void)
{
    uint32_t time_ms = 509; //Time(in miliseconds) between consecutive compare events.
    uint32_t time_ticks;
    uint32_t err_code = NRF_SUCCESS;
    
    //Configure all leds on board.
    LEDS_CONFIGURE(LEDS_MASK);
    LEDS_OFF(LEDS_MASK);
    
    //Configure TIMER_LED for generating simple light effect - leds on board will invert his state one after the other.
    err_code = nrf_drv_timer_init(&TIMER_LED, NULL, timer_led_event_handler);
    APP_ERROR_CHECK(err_code);
    
    time_ticks = nrf_drv_timer_ms_to_ticks(&TIMER_LED, time_ms);
    
    nrf_drv_timer_extended_compare(
         &TIMER_LED, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
    
    nrf_drv_timer_enable(&TIMER_LED);
}
