#ifndef MOTORINTERFACE_H
#define MOTORINTERFACE_H

typedef enum
{
    CLOCKWISE = 1,
    ANTICLOCKWISE = 2,
    STOP = 3
}MotorStateEnum_t;

typedef struct 
{
    int state;
    int speed;
}MotorState_t;

typedef void *motor_handle_t;

typedef struct 
{
    int (*init)(motor_handle_t handle); // Initialize the motor
    void (*runFwd)(motor_handle_t handle, int speed); // Run the motor at a given speed
    void (*runRev)(motor_handle_t handle, int speed); // Run the motor in reverse at a given speed
    void (*stop)(motor_handle_t handle); // Stop the motor
    const char *type_name; // Type name of the motor driver
}MotorDriver;

#endif // MOTORINTERFACE_H