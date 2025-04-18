#include <avr/wdt.h>
#include <Arduino.h>
#include "Application.h"
#include "Ultrasonic_HCSR04.h"
#include "Toggle_Switch.h"
#include "DriverMotor_AFM.h"

// -- Constants --
#define NUM_APPS 2

// -- Contexts --
static SensorPinContext sensor_contexts[NUM_APPS] = {
    {A0, A1},
    {A2, A3}
};

static SwitchContext switch_contexts[NUM_APPS] = {
    {A4, false, true, 0},
    {A5, false, true, 0}
};

static DriverMotorContext_t prop_motor_contexts[NUM_APPS] = {
    {1}, {3}
};

static DriverMotorContext_t winch_motor_contexts[NUM_APPS] = {
    {2}, {4}
};

static ApplicationHandle_t app_handles[NUM_APPS];
static Application* apps[NUM_APPS];  // Pointers to Application objects

// -- Time --
static uint64_t last_time = 0;

// -- RAM Check --
int freeMemory()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

// -- Initialization per app --
void initializeAppHandle(int i)
{
    app_handles[i].sensor_handle = (sensor_handle_t)&sensor_contexts[i];
    app_handles[i].switch_handle = (switch_handle_t)&switch_contexts[i];
    app_handles[i].motor_handle_prop = (motor_handle_t)&prop_motor_contexts[i];
    app_handles[i].motor_handle_winch = (motor_handle_t)&winch_motor_contexts[i];

    app_handles[i].water_level_sensor = &ultrasonic_hcsr04_driver;
    app_handles[i].toggle_switch_driver = &toggle_switch_driver;
    app_handles[i].motor_propeller = &afm_motor_prop;
    app_handles[i].motor_winch = &afm_motor_winch;

    apps[i] = new Application(app_handles[i]);
}

// -- Print App Status --
void printAppStatus(uint32_t &live_count)
{
    Serial.println();
    Serial.print("[main] live count: ");
    Serial.print(live_count++);
    Serial.print("\t[RAM] Free RAM: ");
    Serial.print(freeMemory());
    Serial.println(" bytes");

    for (int i = 0; i < NUM_APPS; i++)
    {
        Serial.print("> ");

        apps[i]->update_sensor();
        int water_level = apps[i]->get_water_level();
        Serial.print("Water level ");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(water_level);
        Serial.print(" cm\t");

        Serial.print("Prop ");
        Serial.print(i + 1);
        Serial.print(" speed: ");
        Serial.print(apps[i]->get_prop_speed());

        int winch_state = apps[i]->get_winch_state();
        Serial.print("\tWinch ");
        Serial.print(i + 1);
        Serial.print(" rot: (");
        Serial.print(winch_state);
        Serial.print(") ");
        switch (winch_state)
        {
            case 1: Serial.print("CW"); break;
            case 2: Serial.print("CCW"); break;
            case 3: Serial.print("Stop"); break;
            default: Serial.print("Invalid!"); break;
        }

        Serial.println();
    }
}

// -- Arduino Setup --
void setup()
{
    wdt_disable();
    Serial.begin(9600);
    delay(1000);

    for (int i = 0; i < NUM_APPS; i++)
    {
        initializeAppHandle(i);
    }

    wdt_enable(WDTO_2S);
    Serial.println("[main] Starting applications...");

    for (int i = 0; i < NUM_APPS; i++)
    {
        if (!apps[i]->init())
        {
            Serial.print("[main] Application ");
            Serial.print(i + 1);
            Serial.println(" initialization failed!");
        }
    }
}

// -- Arduino Loop --
void loop()
{
    wdt_reset();

    for (int i = 0; i < NUM_APPS; i++)
    {
        apps[i]->update_switch_state();
        apps[i]->enable_motor_winch();
        apps[i]->enable_motor_propeller(20);
    }

    static uint32_t live_count = 0;
    uint64_t current_time = millis();

    if (current_time - last_time >= 1000)
    {
        printAppStatus(live_count);
        last_time = current_time;
    }
}
