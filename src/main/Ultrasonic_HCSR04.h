#ifndef ULTRASONIC_HCSR04_H
#define ULTRASONIC_HCSR04_H

#include "SensorInterface.h"

typedef struct
{
    int trigger_pin; // Trigger pin number
    int echo_pin; // Echo pin number
} SensorPinContext;

extern SensorDriver ultrasonic_hcsr04_driver;

#endif // ULTRASONIC_HCSR04_H