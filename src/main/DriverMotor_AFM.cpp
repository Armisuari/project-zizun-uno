#include "DriverMotor_AFM.h"
#include <Arduino.h>

// static MotorState_t *motor_state = nullptr;

int motor_init(motor_handle_t handle)
{
    // motor_state = (MotorState_t*) data;
    DriverMotorContext_t *motor_ctx = (DriverMotorContext_t *)handle;
    if (motor_ctx == NULL)
    {
        Serial.println("[AFMotor] Error: Global context is NULL");
        return -1; // Return -1 if context is NULL
    }
    motor_ctx->motor.setSpeed(0); // Set initial speed to 0
    motor_ctx->motor.run(RELEASE); // Release the motor
    // motor_state->state = STOP;

    return 0; // Return 0 for success
}

void motor_runFwd(motor_handle_t handle, int speed)
{
    DriverMotorContext_t *motor_ctx = (DriverMotorContext_t *)handle;
    if (motor_ctx == NULL)
    {
        Serial.println("[AFMotor] Error: Global context is NULL");
        return; // Return if context is NULL
    }
    
    // motor_state->speed = speed;
    motor_ctx->motor.setSpeed(speed); // Set the speed
    motor_ctx->motor.run(FORWARD); // Run the motor forward
    // motor_state->state = CLOCKWISE;
}

void motor_runRev(motor_handle_t handle, int speed)
{
    DriverMotorContext_t *motor_ctx = (DriverMotorContext_t *)handle;
    if (motor_ctx == NULL)
    {
        Serial.println("[AFMotor] Error: Global context is NULL");
        return; // Return if context is NULL
    }

    // motor_state->speed = speed;
    motor_ctx->motor.setSpeed(speed); // Set the speed
    motor_ctx->motor.run(BACKWARD); // Run the motor backward
    // motor_state->state = ANTICLOCKWISE;
}

void motor_stop(motor_handle_t handle)
{
    DriverMotorContext_t *motor_ctx = (DriverMotorContext_t *)handle;
    if (motor_ctx == NULL)
    {
        Serial.println("[AFMotor] Error: Global context is NULL");
        return; // Return if context is NULL
    }

    // motor_state->speed = 0;
    motor_ctx->motor.setSpeed(0); // Set speed to 0
    motor_ctx->motor.run(RELEASE); // Release the motor
    // motor_state->state = STOP;
}

MotorDriver afm_motor_prop = {
    .init = motor_init,
    .runFwd = motor_runFwd,
    .runRev = motor_runRev,
    .stop = motor_stop,
    .type_name = "AFMotor" // Type name of the motor driver
};

MotorDriver afm_motor_winch = {
    .init = motor_init,
    .runFwd = motor_runFwd,
    .runRev = motor_runRev,
    .stop = motor_stop,
    .type_name = "AFMotor" // Type name of the motor driver
};
