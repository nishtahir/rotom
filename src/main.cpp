#include <FreeRTOS.h>
#include <task.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/gpio.h>
#include "usb.h"
#include "sketch.h"
#include "time.h"

/*
 * Handler in case our application overflows the stack
 */
void vApplicationStackOverflowHook(
    TaskHandle_t xTask __attribute__((unused)),
    char *pcTaskName __attribute__((unused)))
{
  for (;;)
  {
    // No-op
  }
}

/**
 * Handler to poll usb and service requests from the host
 */
void poll_usb_task(void *args __attribute__((unused)))
{
  for (;;)
  {
    usb_poll();
  }
}

/**
 * Update loop to prepare controller state for dispatch
 */
void update_task(void *args __attribute__((unused)))
{
  for (;;)
  {
    update();
  }
}

/**
 * Dispatch controller updates
 */
void dispatch_task(void *args __attribute__((unused)))
{
  for (;;)
  {
    dispatch();
  }
}

/**
 * Init and set main sys clock
 */
static void sys_tick_init()
{
  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
  systick_interrupt_enable();
  systick_counter_enable();
}

int main(void)
{
  // RCC Set System Clock PLL at 72MHz from
  // High speed external oscillator
  rcc_clock_setup_in_hse_8mhz_out_72mhz();

  // LED_BUILTIN is on GPIO bank C.
  // Enable it for use.
  rcc_periph_clock_enable(RCC_GPIOC);

  // Configure pinmode as output for LED_BUILTIN - PC13
  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_OPENDRAIN, GPIO13);
  gpio_toggle(GPIOC, GPIO13);

  // Setup system clocks
  sys_tick_init();

  // Setup USB
  usb_init();
  gpio_toggle(GPIOC, GPIO13);

  // TODO - we probably want to poll the USB  and kick off the
  // other tasks when the device is available
  xTaskCreate(poll_usb_task, "usb_task", 100, NULL, 2, NULL);

  // Setup sketch
  setup();

  // Start the update and dispatch tasks
  xTaskCreate(dispatch_task, "dispatch_task", 100, NULL, 2, NULL);
  xTaskCreate(update_task, "update_task", 100, NULL, 2, NULL);

  vTaskStartScheduler();

  // The task scheduler is blocking,
  // we should never get here
  for (;;)
  {
    // No-op
  }

  return 0;
}