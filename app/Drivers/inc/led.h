#ifndef led_h
#define led_h

// typedef enum {
//     invalidColor = -1,
//     green,
//     red,
//     blue
// } ledColor_t;

void led_On(Collection_t coll);
void led_Off (Collection_t coll);
void led_Blink (Collection_t coll);
void led_ColorChange (Collection_t coll, ledColors_t newColor);
void led_activateRedLed();
void led_activateBlueLed();
void led_activateGreenLed();

#endif