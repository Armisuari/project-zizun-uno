#ifndef SWITCHIMPL_H
#define SWITCHIMPL_H

#include "SensorInterface.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        int pin;
        bool switchState;
        bool lastReading;
        unsigned long lastDebounceTime;
    } SwitchContext;

    extern SwitchDriver toggle_switch_driver;

#ifdef __cplusplus
}
#endif

#endif // SWITCHIMPL_H
