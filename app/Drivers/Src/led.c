#include "led.h"
#include "main.h"
#include <stdint.h>
#include <string.h>
#include "stm32f207xx.h"


#define NUM_LEDS 3
#define NUM_COLLS 1
#define BLINK_THRESHOLD 1000

typedef enum {
    ledColors_Red             = 0x1,
    ledColors_Blue            = 0x2,
    ledColors_Green           = 0x4,
    ledColors_Magenta         = ledColors_Red | ledColors_Blue,
    ledColors_Yellow          = ledColors_Green | ledColors_Red,
    ledColors_Cyan            = ledColors_Green | ledColors_Blue,
    ledColors_White           = ledColors_Red | ledColors_Green | ledColors_Blue
} ledColors_t;

typedef enum {
    ledBehavior_Drive,
    ledBehavior_Blinking 
} ledBehavior_t;

typedef enum {
    ledCollectionState_Off,
    ledCollectionState_On
} ledCollectionState_t;

typedef struct led {
    uint16_t pin; //GPIO Pin of led
    GPIO_TypeDef *port; //GPIO port of led
} led_t;

//NOTE may need larger values for count/thresh
typedef struct blinkInfo {
    int blinkCount;
    int blinkThreshold; 
    int numBlinks;
} BlinkInfo_t;

typedef struct collection {
    led_t leds[NUM_LEDS];
    ledColors_t color;
    BlinkInfo_t blink;
    ledCollectionState_t state;
    ledBehavior_t behavior; 
} Collection_t;



// static  Led_t ledArray[NUM_LEDS] = {

//     [ledColors_Green] = 
//     {
//         .pin = ledG_Pin,
//         .port = ledG_GPIO_Port
//     },

//     [ledColors_Red] = 
//     {
//         .pin = ledR_Pin,
//         .port = ledR_GPIO_Port
//     },

//     [ledColors_Blue] = 
//     {
//         .pin = ledB_Pin,
//         .port = ledB_GPIO_Port
//     }
// };

static Collection_t collArray[NUM_COLLS] = {
    [0] =
    {
        .leds = {
                    [0] = 
                    {
                        .pin = ledG_Pin,
                        .port = ledG_GPIO_Port
                    },

                    [1] = 
                    {
                        .pin = ledR_Pin,
                        .port = ledR_GPIO_Port
                    },

                    [2] = 
                    {
                        .pin = ledB_Pin,
                        .port = ledB_GPIO_Port
                    }
                },
        .color = ledColors_White,
        .blink = {.blinkCount = 0, .blinkThreshold = BLINK_THRESHOLD, .numBlinks = 10}
    }
};


//turn on the led indefinitely
void led_activateLed(Collection_t coll)
{
    int i;
    for(i = 0; i < NUM_LEDS; i++)
    {
        if ()
        {
            HAL_GPIO_WritePin(coll.leds[], GPIO_PIN_SET);
        }
    }
    
    return;
}

void led_activateRedLed()
{
    led_activateLed(ledColors_Red);
    return;
}

void led_activateBlueLed()
{
    led_activateLed(ledColors_Blue);
    return;
}

void led_activateGreenLed()
{
    led_activateLed(ledColors_Green);
    return;
}

void led_resetLeds()
{
    int i;
    for (i = 0; i < NUM_LEDS; i++)
    {
        if (HAL_GPIO_ReadPin(ledArray[i].port, ledArray[i].pin) == GPIO_PIN_SET)
        {
            HAL_GPIO_WritePin(ledArray[i].port, ledArray[i].pin, GPIO_PIN_RESET);
        }
    }
    return;
}