#ifndef SWITCHIMPL_H
#define SWITCHIMPL_H

#include "SensorInterface.h"

typedef struct
{
    int pin;
    bool switchState;
    bool lastReading;
    unsigned long lastDebounceTime;
} SwitchContext;

extern SwitchDriver toggle_switch_driver;

#endif // SWITCHIMPL_H
