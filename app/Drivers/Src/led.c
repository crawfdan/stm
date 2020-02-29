#include "led.h"
#include "main.h"
#include <stdint.h>
#include <string.h>
#include "stm32f207xx.h"


#define NUM_LEDS 3
#define NUM_COLLS 1
#define BLINK_THRESHOLD 1000


typedef struct led {
    uint16_t pin; //GPIO Pin of led
    GPIO_TypeDef *port; //GPIO port of led
} Led_t;

typedef struct blinkInfo {
    int blinkCount;
    int blinkThreshold;
} BlinkInfo_t;

typedef struct collection {
    Led_t leds[NUM_LEDS];
    ledColors_t color;
    BlinkInfo_t blink;
} Collection_t;

typedef enum {
    invalidColor = 0x0,
    red = 0x1,
    blue = 0x2,
    magenta = red | blue,
    green = 0x4,
    yellow = green | red,
    cyan = green | blue,
    white = red | green | blue
} ledColors_t;

// static  Led_t ledArray[NUM_LEDS] = {
//     [green] = 
//     {
//         .pin = ledG_Pin,
//         .port = ledG_GPIO_Port
//     },

//     [red] = 
//     {
//         .pin = ledR_Pin,
//         .port = ledR_GPIO_Port
//     },

//     [blue] = 
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
        .color = invalidColor,
        .blink = {.blinkCount = 0, .blinkThreshold = BLINK_THRESHOLD}
    }
};



void led_activateLed(ledColor_t color)
{
    //led_resetLeds();
    HAL_GPIO_WritePin(ledArray[color].port, ledArray[color].pin, GPIO_PIN_SET);
    return;
}

void led_activateRedLed()
{
    led_activateLed(red);
    return;
}

void led_activateBlueLed()
{
    led_activateLed(blue);
    return;
}

void led_activateGreenLed()
{
    led_activateLed(green);
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