#include <avr/wdt.h>
#include <Arduino.h>
#include "Application.h"
#include "Ultrasonic_HCSR04.h"
#include "Toggle_Switch.h"
#include "DriverMotor_AFM.h"

// Contexts and handles
static SensorPinContext ultrasonic_hcsr04_driver_context_1 = {A0, A1}; // Trigger pin A0, Echo pin A1
static SwitchContext switch_context_1 = {A4, false, true, 0};          // Switch 1: Pin A4
static DriverMotorContext_t afm_motor_driver_ctx_prop_1 = {1};         // Motor 1
static DriverMotorContext_t afm_motor_driver_ctx_winch_1 = {2};        // Motor 2
static ApplicationHandle_t app_handle_1;

Application app_1(app_handle_1); // Application instance

uint64_t last_time = 0; // Last sensor update time

// Function to calculate free memory
int freeMemory()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

// Function to initialize application handle
void initializeAppHandle()
{
    app_handle_1.sensor_handle = (sensor_handle_t)&ultrasonic_hcsr04_driver_context_1;
    app_handle_1.switch_handle = (switch_handle_t)&switch_context_1;
    app_handle_1.motor_handle_prop = (motor_handle_t)&afm_motor_driver_ctx_prop_1;
    app_handle_1.motor_handle_winch = (motor_handle_t)&afm_motor_driver_ctx_winch_1;

    app_handle_1.water_level_sensor = &ultrasonic_hcsr04_driver;
    app_handle_1.toggle_switch_driver = &toggle_switch_driver;
    app_handle_1.motor_propeller = &afm_motor_prop;
    app_handle_1.motor_winch = &afm_motor_winch;
}

// Function to print application status
void printAppStatus(uint32_t &live_count)
{
    Serial.print("[main] live count: ");
    Serial.print(live_count++);
    Serial.print("\t");

    app_1.update_sensor();

    int water_level = app_1.get_water_level();
    Serial.print("Water level 1: ");
    Serial.print(water_level);
    Serial.print(" cm\t");

    Serial.print("Prop 1 speed: ");
    Serial.print(app_1.get_prop_speed());

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

    Serial.print("\t[RAM] Free RAM: ");
    Serial.print(freeMemory());
    Serial.println(" bytes");
}

void setup()
{
    wdt_disable(); // Disable WDT

    initializeAppHandle();

    Serial.begin(9600);
    delay(1000); // Wait for serial connection

    wdt_enable(WDTO_2S); // Set WDT timeout to 2 seconds

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
    wdt_reset(); // Reset watchdog timer

    app_1.update_switch_state();
    app_1.enable_motor_winch();
    app_1.enable_motor_propeller(20);

    uint64_t current_time = millis();
    if (current_time - last_time >= 1000)
    {
        static uint32_t live_count = 0;
        printAppStatus(live_count);
        last_time = current_time;
    }
}