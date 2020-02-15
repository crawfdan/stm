#ifndef led_h
#define led_h

typedef enum {
    invalidColor = -1,
    green,
    red,
    blue
} ledColor_t;

void led_activateLed(void);
void led_resetLeds();

#endif