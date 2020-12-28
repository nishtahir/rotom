#include <FreeRTOS.h>
#include <task.h>
#include <stdlib.h>
#include <string.h>
#include "usb.h"
#include "sketch.h"
#include "controller.h"

#define _ITERATIONS 1000

Command script[] = {
    {A, 5},
};

JoyConReport controller;

void delay(uint64_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void reset_controls()
{
    // Zero out controller data
    memset(&controller, 0, sizeof(JoyConReport));
    controller.LX = STICK_CENTER;
    controller.LY = STICK_CENTER;
    controller.RX = STICK_CENTER;
    controller.RY = STICK_CENTER;
    controller.HAT = HAT_CENTER;
}

void hold_and_reset(uint64_t ms)
{
    delay(ms);
    reset_controls();
    delay(500);
}

void push_and_delay(uint64_t ms)
{
    delay(300);
    reset_controls();
    delay(ms);
}

void setup()
{
    reset_controls();
}

void update()
{
    // Initial delay to account for device initialization
    delay(3000);

    for (int i = 0; i < _ITERATIONS; i++)
    {
        for (auto &command : script)
        {

            switch (command.button)
            {

            case UP:
                controller.LY = STICK_MIN;
                break;

            case LEFT:
                controller.LX = STICK_MIN;
                break;

            case DOWN:
                controller.LY = STICK_MAX;
                break;

            case RIGHT:
                controller.LX = STICK_MAX;
                break;

            case A:
                controller.Buttons |= SWITCH_A;
                break;

            case B:
                controller.Buttons |= SWITCH_B;
                break;

            case X:
                controller.Buttons |= SWITCH_X;
                break;

            case Y:
                controller.Buttons |= SWITCH_Y;
                break;

            case L:
                controller.Buttons |= SWITCH_L;
                break;

            case R:
                controller.Buttons |= SWITCH_R;
                break;

            case PLUS:
                controller.Buttons |= SWITCH_PLUS;
                break;

            case TRIGGERS:
                controller.Buttons |= SWITCH_L | SWITCH_R;
                break;

            case UP_LEFT:
                controller.RX = STICK_MIN;
                controller.LX = STICK_MIN;
                break;

            case UP_RIGHT:
                controller.LY = STICK_MIN;
                controller.RX = STICK_MAX;
                break;

            default:
                controller.LX = STICK_CENTER;
                controller.LY = STICK_CENTER;
                controller.RX = STICK_CENTER;
                controller.RY = STICK_CENTER;
                controller.HAT = HAT_CENTER;
                break;
            }
            hold_and_reset(command.duration * 10);
        }
    }

    for (;;)
    {
        // Script execution is complete. Trap
    }
}

void dispatch()
{
    // Initial delay to allow device to connect
    delay(2000);
    for (;;)
    {
        // Send report data
        usb_write((uint8_t *)&controller, sizeof(JoyConReport));
        // To avoid flooding the device
        delay(10);
    }
}
