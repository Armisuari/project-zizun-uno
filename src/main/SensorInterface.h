#ifndef SENSORINTERFACE_H
#define SENSORINTERFACE_H

#include <stdbool.h>

typedef void *sensor_handle_t;
typedef void *switch_handle_t;

typedef struct
{
    int distance; // Distance in cm
} WaterLevelData_t;

typedef struct
{
    bool isPushedUp; // true = pushed up, false = pushed down
} SwitchState_t;

typedef struct
{
    int (*init)(sensor_handle_t handle);
    bool (*detect)(sensor_handle_t handle);
    void (*read)(sensor_handle_t handle, const void *data);
    const char *type_name;
} SensorDriver;

typedef struct
{
    int (*init)(switch_handle_t handle);                    // Initialize the switch
    void (*read)(switch_handle_t handle, const void *data); // Read (e.g., toggle) the switch state
} SwitchDriver;

#endif // SENSORINTERFACE_H
