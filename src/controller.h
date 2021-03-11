#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <stdlib.h>

// Sourced and adapted from
// https://github.com/bertrandom/snowball-thrower/blob/master/Joystick.h
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
} JoyConButtons;

struct JoyConReport
{
    uint16_t Buttons; // 16 buttons; see JoyConButtons for bit mapping
    uint8_t HAT;      // HAT switch; one nibble w/ unused nibble
    uint8_t LX;       // Left  Stick X
    uint8_t LY;       // Left  Stick Y
    uint8_t RX;       // Right Stick X
    uint8_t RY;       // Right Stick Y
    uint8_t VendorSpec;
};

enum ControlButtons
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
    PLUS,
    NOTHING,
    TRIGGERS,
    UP_RIGHT,
    UP_LEFT,
    SPIN,
};

struct Command
{
    ControlButtons button;
    uint16_t duration;
};

#endif