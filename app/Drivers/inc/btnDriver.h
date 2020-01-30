#ifndef btnDriver_h
#define btnDriver_h

#define NUM_BTNS 1

#include "stm32f2xx_hal_gpio.h"
#include <stdint.h>

typedef enum {
    invalidState = -1,
    notPressed,
    pressed,
    longPress
} State;

typedef struct button {
    uint16_t pinVal;
    State btnState;
    State prevButtonState;
    int debounceCount;
    int debounceThreshold;
    int holdCount;
    int holdThreshold;
    void (* pressedCallback)();
    void (*longPressCallback)();
    void (*unpressedCallback)();
} Button;

#endif