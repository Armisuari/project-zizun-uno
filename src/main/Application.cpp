#include "Application.h"

Application::Application(ApplicationHandle_t &app_handle) : app_handle(app_handle) {}

static int init_water_level_sensor(ApplicationHandle_t &app_handle) { return app_handle.water_level_sensor->init(app_handle.sensor_handle); }
static int init_toggle_switch_driver(ApplicationHandle_t &app_handle) { return app_handle.toggle_switch_driver->init(app_handle.switch_handle); }
static int init_motor_driver_propeller(ApplicationHandle_t &app_handle) { return app_handle.motor_propeller->init(app_handle.motor_handle_prop); }
static int init_motor_driver_winch(ApplicationHandle_t &app_handle) { return app_handle.motor_winch->init(app_handle.motor_handle_winch); }

bool Application::init()
{
    struct InitComponent
    {
        const char *name;
        int (*init_func)(ApplicationHandle_t &);
    };

    InitComponent components[] = {
        {"water level sensor", init_water_level_sensor},
        {"toggle switch driver", init_toggle_switch_driver},
        {"motor driver for propeller", init_motor_driver_propeller},
        {"motor driver for winch", init_motor_driver_winch},
    };

    for (const auto &component : components)
    {
        if (component.init_func(app_handle) != 0)
        {
            Serial.print("[Application] Error initializing ");
            Serial.println(component.name);
            return false;
        }
        Serial.print("[Application] ");
        Serial.print(component.name);
        Serial.println(" initialized successfully");
    }

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
    // Map water level distance to motor speed
    motor_prop.speed = map(water_level_data.distance, 0, 100, 255, 0);

    // Determine motor action based on water level threshold
    if (water_level_data.distance < threshold)
    {
        app_handle.motor_propeller->runFwd(app_handle.motor_handle_prop, motor_prop.speed);
    }
    else
    {
        app_handle.motor_propeller->stop(app_handle.motor_handle_prop);
    }
}
