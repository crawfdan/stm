#ifndef led_h
#define led_h

#include <stdbool.h>

typedef enum {
    ledColors_Red             = 0x1,
    ledColors_Green           = 0x2,
    ledColors_Blue            = 0x4,
    ledColors_Magenta         = ledColors_Red | ledColors_Blue,
    ledColors_Yellow          = ledColors_Green | ledColors_Red,
    ledColors_Cyan            = ledColors_Green | ledColors_Blue,
    ledColors_White           = ledColors_Red | ledColors_Green | ledColors_Blue
} ledColors_t;

typedef enum {
    invalidCollection = -1,
    ledCollections_1
} ledCollections_t;

typedef struct collection Collection_t;

void led_On(ledCollections_t index);
void led_Off (ledCollections_t index);
bool led_Blink (ledCollections_t index);
void led_StopBlink (ledCollections_t index);
void led_ColorChange (ledCollections_t index, ledColors_t newColor);
void led_taskCreate();
void led_activateRedLed();
void led_activateBlueLed();
void led_activateGreenLed();

///////////////////////////////////////////////////////
//TEST FUNCTIONS
///////////////////////////////////////////////////////
void led_test2();
void led_test();
ledColors_t nextColor();
bool blink();
void speedUp();

#endif