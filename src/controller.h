#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdlib.h>

// Sourced from https://github.com/bertrandom/snowball-thrower/blob/master/Joystick.h
// Type Defines
// Enumeration for joystick buttons.
typedef enum
{
    SWITCH_Y = 0x01,
    SWITCH_B = 0x02,
    SWITCH_A = 0x04,
    SWITCH_X = 0x08,
    SWITCH_L = 0x10,
    SWITCH_R = 0x20,
    SWITCH_ZL = 0x40,
    SWITCH_ZR = 0x80,
    SWITCH_MINUS = 0x100,
    SWITCH_PLUS = 0x200,
    SWITCH_LCLICK = 0x400,
    SWITCH_RCLICK = 0x800,
    SWITCH_HOME = 0x1000,
    SWITCH_CAPTURE = 0x2000,
} JoystickButtons_t;

#define HAT_TOP 0x00
#define HAT_TOP_RIGHT 0x01
#define HAT_RIGHT 0x02
#define HAT_BOTTOM_RIGHT 0x03
#define HAT_BOTTOM 0x04
#define HAT_BOTTOM_LEFT 0x05
#define HAT_LEFT 0x06
#define HAT_TOP_LEFT 0x07
#define HAT_CENTER 0x08

#define STICK_MIN 0
#define STICK_CENTER 128
#define STICK_MAX 255

// Joystick HID report structure. We have an input and an output.
typedef struct
{
    uint16_t Button; // 16 buttons; see JoystickButtons_t for bit mapping
    uint8_t HAT;     // HAT switch; one nibble w/ unused nibble
    uint8_t LX;      // Left  Stick X
    uint8_t LY;      // Left  Stick Y
    uint8_t RX;      // Right Stick X
    uint8_t RY;      // Right Stick Y
    uint8_t VendorSpec;
} USB_JoystickReport_Input_t;

// The output is structured as a mirror of the input.
// This is based on initial observations of the Pokken Controller.
typedef struct
{
    uint16_t Button; // 16 buttons; see JoystickButtons_t for bit mapping
    uint8_t HAT;     // HAT switch; one nibble w/ unused nibble
    uint8_t LX;      // Left  Stick X
    uint8_t LY;      // Left  Stick Y
    uint8_t RX;      // Right Stick X
    uint8_t RY;      // Right Stick Y
} USB_JoystickReport_Output_t;

typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    X,
    Y,
    A,
    B,
    L,
    R,
    THROW,
    NOTHING,
    TRIGGERS
} Buttons_t;

typedef struct
{
    Buttons_t button;
    uint16_t duration;
} command;

union SwitchController
{
    struct
    {
        // byte1
        unsigned int Y : 1;
        unsigned int B : 1;
        unsigned int A : 1;
        unsigned int X : 1;
        unsigned int L : 1;
        unsigned int R : 1;
        unsigned int ZL : 1;
        unsigned int ZR : 1;
        // byte2
        unsigned int minus : 1;
        unsigned int plus : 1;
        unsigned int lclick : 1;
        unsigned int rclick : 1;
        unsigned int home : 1;
        unsigned int capture : 1;
        unsigned int : 2; // unused bits
        // byte3
        unsigned int HAT : 4;
        unsigned int : 4; // unused bits
        // byte4
        uint8_t LX : 8;
        // byte5
        uint8_t LY : 8;
        // byte6
        uint8_t RX : 8;
        // byte7
        uint8_t RY : 8;
        // byte8
        unsigned int : 8; // vendor specific

        // custom/temp data
        unsigned int : 4;
        unsigned int dpad_left : 1;
        unsigned int dpad_right : 1;
        unsigned int dpad_down : 1;
        unsigned int dpad_up : 1;
    } data;
    uint8_t bytes[9];
};

#endif