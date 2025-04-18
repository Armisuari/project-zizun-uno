#ifndef DRIVERMOTOR_AFM_H
#define DRIVERMOTOR_AFM_H

#include "DriverMotorInterface.h"
#include <AFMotor.h>

// #ifdef __cplusplus
// extern "C"
// {
// #endif

typedef struct 
{
    AF_DCMotor motor; // AF_DCMotor object
}DriverMotorContext_t;

extern MotorDriver afm_motor_prop; // Declare the motor driver
extern MotorDriver afm_motor_winch; // Declare the motor driver

// #ifdef __cplusplus
// }
// #endif

#endif // DRIVERMOTOR_AFM_H