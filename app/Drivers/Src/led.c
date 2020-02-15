#include "led.h"
#include "main.h"
#include <stdint.h>
#include "stm32f207xx.h"

#define NUM_LEDS 3

typedef struct led {
    uint16_t pin; //GPIO Pin of led
    GPIO_TypeDef *port; //GPIO port of led
} Led_t;



static  Led_t ledArray[NUM_LEDS] = {
    [green] = 
    {
        .pin = ledG_Pin,
        .port = ledG_GPIO_Port
    },

    [red] = 
    {
        .pin = ledR_Pin,
        .port = ledR_GPIO_Port
    },

    [blue] = 
    {
        .pin = ledB_Pin,
        .port = ledB_GPIO_Port
    }
};

void led_activateLed(ledColor_t color)
{
    led_resetLeds();
    HAL_GPIO_WritePin(ledArray[color].port, ledArray[color].pin, GPIO_PIN_SET);
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