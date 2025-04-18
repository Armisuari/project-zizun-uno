#ifndef APPLICATION_H
#define APPLICATION_H

#include <Arduino.h>
#include "SensorInterface.h"
#include "DriverMotorInterface.h"

typedef struct 
{
    sensor_handle_t sensor_handle;                // Array of sensor handles
    switch_handle_t switch_handle;               // Array of switch handles
    motor_handle_t motor_handle_prop;                // Array of motor handles
    motor_handle_t motor_handle_winch;                // Array of motor handles
    SensorDriver* water_level_sensor;
    SwitchDriver* toggle_switch_driver;
    MotorDriver* motor_propeller; // Pointer to the motor driver
    MotorDriver* motor_winch; // Pointer to the motor driver
}ApplicationHandle_t;

class Application
{
    public:
        Application(ApplicationHandle_t &app_handle);;
        bool init();
        void update_sensor();
        void update_switch_state();

        int get_water_level() { return water_level_data.distance; } // Get the water level distance
        bool get_switch_state() { return switch_state.isPushedUp; } // Get the switch state
        int get_prop_speed() { return  motor_prop.speed; }
        int get_winch_state() { return motor_winch.state; }

        void enable_motor_propeller(int threshold = 20); // Enable the motor propeller with a threshold
        void enable_motor_winch();

    private:
        ApplicationHandle_t &app_handle; // Reference to the application handle
        WaterLevelData_t water_level_data; // Data structure for water level data
        SwitchState_t switch_state; // Data structure for switch state
        MotorState_t motor_prop;
        MotorState_t motor_winch;
};

// #ifdef __cplusplus
// }
// #endif

#endif // APPLICATION_H
