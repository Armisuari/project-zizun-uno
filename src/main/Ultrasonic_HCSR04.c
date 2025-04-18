#include "Ultrasonic_HCSR04.h"
#include <Arduino.h> // Include Arduino library for pin manipulation
#include <stdio.h>   // Include standard I/O library for printf

int hcsr04_init(sensor_handle_t handle) {
    SensorPinContext *context = (SensorPinContext *)handle;
    if (context == NULL) {
        return -1; // Return -1 if context is NULL
    }

    pinMode(context->trigger_pin, OUTPUT); // Set trigger pin as output
    pinMode(context->echo_pin, INPUT); // Set echo pin as input
    return 0; // Return 0 to indicate success
}

bool hcsr04_detect(sensor_handle_t handle) {
    if (handle == NULL) {
        return false; // Return false if handle is NULL
    }
    return true; // Always return true for this sensor
}

void hcsr04_read(sensor_handle_t handle, const void *data) {
    SensorPinContext *context = (SensorPinContext *)handle;
    WaterLevelData_t *water_level_data = (WaterLevelData_t *)data;

    if (context == NULL || water_level_data == NULL) {
        return; // Return if context or data is NULL
    }

    // Trigger the ultrasonic sensor
    digitalWrite(context->trigger_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(context->trigger_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(context->trigger_pin, LOW);

    // Measure the echo duration
    long duration;
    duration = pulseIn(context->echo_pin, HIGH, 10000);
    if (duration == 0) {
        water_level_data->distance = -1; // Indicate an error if no pulse is received
        return;
    }

    // Calculate distance in cm
    water_level_data->distance = duration * 0.034 / 2;
}

SensorDriver ultrasonic_hcsr04_driver = {
    .init = hcsr04_init,
    .detect = hcsr04_detect,
    .read = hcsr04_read,
    .type_name = "Ultrasonic_HCSR04" // Type name of the sensor
};

