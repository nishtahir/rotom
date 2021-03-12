#include <FreeRTOS.h>
#include <task.h>
#include <stdlib.h>
#include <string.h>
#include "usb.h"
#include "sketch.h"
#include "controller.h"

#define SCRIPT_ITERATIONS 100

// Setting the value above 60s seems to cause it to
// overflow. Not sure why this happens so we're spliting the cylcing
// into 2 spin commands for now.
#define HALF_SPIN_DURATION_MS 36000

Command script[] = {
    // Fly
    {A, 100},
    {X, 500},
    {PLUS, 2500},
    {A, 500},
    {A, 300},

    // Talk to day care lady
    {DOWN, 3000},
    {LEFT, 700},
    {A, 1000},
    {A, 3000},
    {A, 3000},

    // Put egg in party
    {A, 1000},
    {A, 1000},
    {NOTHING, 1000},
    {DOWN, 300},
    {A, 1000},
    {A, 2000},
    {A, 2000},
    {NOTHING, 500},
    {A, 300},

    // Hatch
    // Go to spin position
    {UP, 2000},
    {UP_RIGHT, 3000},
    // Hop on bike
    {PLUS, 300},
    {SPIN, HALF_SPIN_DURATION_MS},
    {SPIN, HALF_SPIN_DURATION_MS},
    // Oh?
    {A, 300},
    {NOTHING, 20000},
    // Fade animation
    {A, 3000},
    {NOTHING, 2000},
    // Hop off of bike
    {PLUS, 300},
    {NOTHING, 1000},
};

JoyConReport controller;

/**
 * Suspend the current task and wait
 */
void delay(uint64_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

/**
 * Zero out controller state
 */
void reset_controller()
{
    // Zero out controller data
    memset(&controller, 0, sizeof(JoyConReport));
    controller.LX = STICK_CENTER;
    controller.LY = STICK_CENTER;
    controller.RX = STICK_CENTER;
    controller.RY = STICK_CENTER;
    controller.HAT = HAT_CENTER;
}

/**
 * Update controller state
 */
void update_controller(ControlButtons button)
{
    switch (button)
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
        controller.LY = STICK_MIN;
        controller.LX = STICK_MIN;
        break;

    case UP_RIGHT:
        controller.LY = STICK_MIN;
        controller.LX = STICK_MAX;
        break;

    case SPIN:
        controller.RX = STICK_MIN;
        controller.LX = STICK_MIN;
        break;
    default:
        break;
    }
}

/**
 * Hold down button for a given duration
 */
void hold_button(ControlButtons button, uint64_t duration_ms)
{
    update_controller(button);
    delay(duration_ms);
    reset_controller();
    delay(100);
}

/**
 * Initialize 
 */
void setup()
{
    reset_controller();
}

/**
 * Update controller state
 */
void update()
{
    // Initial delay for device initialization
    delay(1500);
    for (int i = 0; i < SCRIPT_ITERATIONS; i++)
    {
        for (auto &command : script)
        {
            hold_button(command.button, command.duration);
        }
    }

    for (;;)
    {
        // Script execution is complete. Trap
    }
}

/**
 * Write controller state to USB output
 */
void dispatch()
{
    // Initial delay to allow device to connect
    delay(1000);
    for (;;)
    {
        // Send report data
        usb_write((uint8_t *)&controller, sizeof(JoyConReport));
        // To avoid flooding the device
        delay(10);
    }
}
