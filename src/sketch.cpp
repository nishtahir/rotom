#include "sketch.h"

SwitchController controller;

void delay(uint64_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void reset_controls()
{
    controller = SwitchController();
    controller.data.LX = STICK_CENTER;
    controller.data.LY = STICK_CENTER;
    controller.data.RX = STICK_CENTER;
    controller.data.RX = STICK_CENTER;
    controller.data.HAT = HAT_CENTER;
}

void delay_and_reset(uint64_t ms)
{
    delay(ms);
    reset_controls();
    delay(1000);
}

void send_control_data()
{
    usb_write(controller.bytes, 8);
}

void setup()
{
    controller = SwitchController();
}

void update()
{
    // Assume we're on controller grip/order screen
    // delay_and_reset(3000);
    delay(3000);

    controller.data.L = 1;
    controller.data.R = 1;

    delay(1000);
    reset_controls();
    delay(3000);

    // Goto home screen
    controller.data.home = 1;
    delay_and_reset(200);

    // Home again to go to game
    controller.data.home = 1;
    delay_and_reset(200);

    // Wait for game to launch
    delay(3000);

    // Jump three
    controller.data.A = 1;
    delay_and_reset(500);

    controller.data.A = 1;
    delay_and_reset(500);

    controller.data.A = 1;
    delay_and_reset(500);

    for (;;)
        ;
}

void dispatch()
{
    send_control_data();
    delay(10);
}
