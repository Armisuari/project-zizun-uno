#include "Toggle_Switch.h"
#include <Arduino.h>

const unsigned long debounceDelay = 50; // milliseconds

int switch_init(switch_handle_t handle)
{
  SwitchContext *ctx = (SwitchContext *)handle;
  if (ctx == NULL)
  {
    Serial.println("[Switch_init] Error: Global context is NULL");
    return -1; // Return -1 if context is NULL
  }
  Serial.print("[Switch_init] Initializing switch on pin: ");
  Serial.println(ctx->pin);
  pinMode(ctx->pin, INPUT_PULLUP); // Set the pin as input with pull-up resistor
  return 0;
}

void switch_read(switch_handle_t handle, const void *data)
{
  SwitchContext *ctx = (SwitchContext *)handle;
  if (ctx == NULL)
  {
    Serial.println("[Switch] Error: Global context is NULL");
    return;
  }

  bool currentReading = digitalRead(ctx->pin); // Read the switch state

  // If the reading changed from last loop, reset the debounce timer
  if (currentReading != ctx->lastReading)
  {
    ctx->lastDebounceTime = millis();
  }

  // If the reading has been stable for debounceDelay, take it as the actual state
  if ((millis() - ctx->lastDebounceTime) > debounceDelay)
  {
    // Only update state if it changed

    if (currentReading != ctx->switchState)
    {
      ctx->switchState = currentReading;

      Serial.print("[Switch_read] Reading switch on pin: ");
      Serial.println(ctx->pin);
      Serial.print("[Switch] Switch state changed: ");
      Serial.println(ctx->switchState ? "Pushed Down" : "Pushed UP");

      SwitchState_t *state = (SwitchState_t *)data;
      if (state == NULL)
      {
        Serial.println("[Switch_read] Error: State is NULL");
        return;
      }
      state->isPushedUp = ctx->switchState;
    }
  }

  ctx->lastReading = currentReading;
}

SwitchDriver toggle_switch_driver = {
    .init = switch_init,
    .read = switch_read};