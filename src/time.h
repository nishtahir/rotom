#ifndef TIME_H
#define TIME_H

#include <stdint.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>

static void sys_tick_init()
{
    // Set sys_tick clock to main clock
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_interrupt_enable();
    systick_counter_enable();
}

#endif