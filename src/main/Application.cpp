#include "Application.h"

Application::Application(ApplicationHandle_t &app_handle) : app_handle(app_handle) {}

bool Application::init()
{
    // Initialize the water level sensor
    if (app_handle.water_level_sensor->init(app_handle.sensor_handle) != 0)
    {
        Serial.println("[Application] Error initializing water level sensor");
        return false;
    }

    // Initialize the toggle switch driver
    if (app_handle.toggle_switch_driver->init(app_handle.switch_handle) != 0)
    {
        Serial.println("[Application] Error initializing toggle switch driver");
        return false;
    }

    // Initialize the motor driver
    if (app_handle.motor_propeller->init(app_handle.motor_handle_prop) != 0)
    {
        Serial.println("[Application] Error initializing motor driver for propeller");
        return false;
    }
    Serial.println("[Application] Motor driver for propeller initialized successfully");

    if (app_handle.motor_winch->init(app_handle.motor_handle_winch) != 0)
    {
        Serial.println("[Application] Error initializing motor driver for winch");
        return false;
    }
    Serial.println("[Application] Motor driver for winch initialized successfully");

    Serial.println("[Application] Initialization successful");

    return true;
}

void Application::update_sensor()
{
    // Read the water level sensor data
    app_handle.water_level_sensor->read(app_handle.sensor_handle, &water_level_data);
}

void Application::update_switch_state()
{
    // Read the toggle switch state
    // Serial.println("[Application] switch_handle");
    app_handle.toggle_switch_driver->read(app_handle.switch_handle, &switch_state);
}

void Application::enable_motor_winch()
{
    // Enable the motor winch
    int speed = 255; // Initialize speed to 0
    if (switch_state.isPushedUp)
    {
        // If the switch is pushed up, run the motor winch forward
        app_handle.motor_winch->runFwd(app_handle.motor_handle_winch, speed);
        motor_winch.state = 1;
    }
    else
    {
        // If the switch is not pushed up, stop the motor winch
        app_handle.motor_winch->runRev(app_handle.motor_handle_winch, speed);
        motor_winch.state = 2;
    }
}

void Application::enable_motor_propeller(int threshold)
{
    // Calculate the speed based on water level distance
    int speed = (water_level_data.distance >= 0) 
                ? map(water_level_data.distance, 0, 100, 0, 255) 
                : 0;

    motor_prop.speed = speed;
    // Control the motor propeller based on the water level threshold
    if (water_level_data.distance < threshold)
    {
        app_handle.motor_propeller->runFwd(app_handle.motor_handle_prop, speed);
    }
    else
    {
        app_handle.motor_propeller->stop(app_handle.motor_handle_prop);
    }
}
