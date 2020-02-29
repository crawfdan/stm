#ifndef led_h
#define led_h

// typedef enum {
//     invalidColor = -1,
//     green,
//     red,
//     blue
// } ledColor_t;

void led_activateLed(ledColor_t color);
void led_resetLeds();
void led_activateRedLed();
void led_activateBlueLed();
void led_activateGreenLed();

#endif