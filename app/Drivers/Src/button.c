#include "button.h"
#include "led.h"
#include "stm32f207xx.h"
#include "main.h"
#include <stdint.h>

#define NUM_BUTTONS 1
#define DEBOUNCE_THRESHOLD 10
#define HOLD_THRESHOLD 10

typedef enum {
    invalidState = -1,
    unpressed,
    pressed,
    longPress
} buttonState_t;


typedef struct button {
    uint16_t pin; //GPIO Pin
    GPIO_TypeDef *port; //GPIO port
    buttonState_t prevButtonState; //previous button state
    buttonState_t currButtonState; //current button state
    int debounceCount; //number of current debounces
    int debounceThreshold; //debounce trigger
    int holdCount; //number of held cycles
    int holdThreshold; //button held trigger
    void (* unpressedCb)(); //callback for unpressed state
    void (*longPressCb)(); //callback for long press state
} Button_t;


static  Button_t buttonArray[NUM_BUTTONS] = {
    [0] = 
    {
        .pin = pushButton_Pin,
        .port = pushButton_GPIO_Port,
        .prevButtonState = invalidState,
        .currButtonState = invalidState,
        .debounceCount = 0,
        .debounceThreshold = DEBOUNCE_THRESHOLD,
        .holdCount = 0,
        .holdThreshold = HOLD_THRESHOLD,
        .unpressedCb = &led_activateRedLed,
        .longPressCb = &led_activateBlueLed
    }
};

void button_updateButton(void)
{
    int i;

    for(i = 0; i < sizeof(buttonArray); i++)
    {
        //if dbc < threshold and pin set
        if (buttonArray[i].debounceCount < DEBOUNCE_THRESHOLD && 
        HAL_GPIO_ReadPin(buttonArray[i].port, buttonArray[i].pin) == GPIO_PIN_SET)
        {
            buttonArray[i].debounceCount++;
        }
        //if dbc > 0 and pin is not set
        else
        {
            buttonArray[i].debounceCount--;
            buttonArray[i].holdCount = 0;
        }

        //change states
        if (buttonArray[i].debounceCount == 0)
        {
            buttonArray[i].currButtonState = unpressed;
        }
            //state is either pressed or long press
        else if (buttonArray[i].debounceCount == buttonArray[i].debounceThreshold)
        {
            //change to long press state
            if (buttonArray[i].holdCount = buttonArray[i].holdThreshold)
            {
                buttonArray[i].currButtonState = longPress;
            }
            //stay in pressed state, increment hold count
            else
            {
                buttonArray[i].currButtonState = pressed;
                buttonArray[i].holdCount++;
            }
        }
            //no valid state
        else
        {
            buttonArray[i].currButtonState = invalidState;
            assert();
            return;
        }
        
        //callbacks
        if (buttonArray[i].prevButtonState != buttonArray[i].currButtonState)
        {
            if (buttonArray[i].prevButtonState == pressed &&
                buttonArray[i].currButtonState == longPress)
                {
                    buttonArray[i].longPressCb();
                }
            else if (buttonArray[i].prevButtonState == pressed &&
                    buttonArray[i].currButtonState == unpressed)
                {
                    buttonArray[i].unpressedCb();
                }
        }

    }
        
    return;
}