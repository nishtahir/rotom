#ifndef SKETCH_H
#define SKETCH_H

#include <FreeRTOS.h>
#include <task.h>
#include <stdlib.h>
#include <string.h>
#include "usb.h"
#include "sketch.h"
#include "controller.h"

void setup();
void update();
void dispatch();

#endif