#include <avr/wdt.h>

#include <Arduino.h>

#include "Application.h"
#include "Ultrasonic_HCSR04.h"
#include "Toggle_Switch.h"
#include "DriverMotor_AFM.h"

static SensorPinContext ultrasonic_hcsr04_driver_context_1 = {A0, A1}; // Trigger pin A0, Echo pin A1
static SwitchContext switch_context_1 = {A4, false, true, 0};          // Switch 1: Pin A4
static DriverMotorContext_t afm_motor_driver_ctx_prop_1 = {1};         // Motor 1
static DriverMotorContext_t afm_motor_driver_ctx_winch_1 = {2};        // Motor 2


static ApplicationHandle_t app_handle_1;

Application app_1(app_handle_1); // Create an instance of the Application class

uint64_t last_time = 0; // Variable to store the last time the sensor was updated

int freeMemory()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

void setup()
{
    wdt_disable(); // Disable WDT in case it was enabled before (after reset)

    app_handle_1.sensor_handle = (sensor_handle_t)&ultrasonic_hcsr04_driver_context_1;
    app_handle_1.switch_handle = (switch_handle_t)&switch_context_1;
    app_handle_1.motor_handle_prop = (motor_handle_t)&afm_motor_driver_ctx_prop_1;   // Initialize the motor handle
    app_handle_1.motor_handle_winch = (motor_handle_t)&afm_motor_driver_ctx_winch_1; // Initialize the motor handle

    app_handle_1.water_level_sensor = &ultrasonic_hcsr04_driver;
    app_handle_1.toggle_switch_driver = &toggle_switch_driver;
    app_handle_1.motor_propeller = &afm_motor_prop; // Add the motor driver to the application handle
    app_handle_1.motor_winch = &afm_motor_winch;     // Add the motor driver to the application handle

    Serial.begin(9600); // Initialize serial communication
    delay(1000);        // Wait for serial connection to establish

    // Set WDT timeout to 2 seconds
    wdt_enable(WDTO_2S);

    Serial.println("[main] Starting application...");
    if (!app_1.init())
    {
        Serial.println("[main] Application 1 initialization failed!");
        return;
    }

    Serial.println("[main] Application 1 initialized successfully!");
}

void loop()
{
    // Serial.println("[main] Looping...");
    wdt_reset(); // Reset the watchdog timer

    app_1.update_switch_state(); // Update the switch state
    app_1.enable_motor_winch(); // Enable the motor winch if the switch is pushed up
    app_1.enable_motor_propeller(20); // Enable the motor propeller with a threshold of 20 cm
    
    uint64_t current_time = millis(); // Get the current time in milliseconds
    if (current_time - last_time >= 1000)
    { // Update every second
        static uint32_t live_count = 0; // Initialize live count
        Serial.print("[main] live count: "); Serial.print(live_count++); Serial.print("\t");

        app_1.update_sensor();                     // Update the sensor data

        int water_level = app_1.get_water_level(); // Get the water level distance
        Serial.print("Water level 1: ");
        Serial.print(water_level);
        Serial.print(" cm\t");

        Serial.print("Prop 1 speed: "); Serial.print(app_1.get_prop_speed());

        int winch_state = app_1.get_winch_state();
        Serial.print("\tWinch 1 rot: (" + String(winch_state) + ") ");
        switch (winch_state)
        {
        case 1:
            Serial.print("CW");
            break;
        case 2:
            Serial.print("CCW");
            break;
        case 3:
            Serial.print("Stop");
            break;
        
        default:
            Serial.print("Invalid!");
            break;
        }

        Serial.print("\t[RAM] Free RAM: "); Serial.print(freeMemory()); Serial.print(" bytes\n");
        last_time = current_time; // Update the last time
    }

    // Serial.println("[main] Looping done!");
}