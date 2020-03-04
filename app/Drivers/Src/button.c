#include "button.h"
#include "led.h"
#include "stm32f207xx.h"
#include "main.h"
#include <stdint.h>

#define NUM_BUTTONS 1
#define DEBOUNCE_THRESHOLD 10
#define HOLD_THRESHOLD 200

typedef enum {
    invalidState = -1,
    unpressed,
    pressed,
    longPress
} buttonState_t;

typedef enum {
    invalidButton = -1,
    button0,
} buttonNames_t;


typedef struct button {
    uint16_t pin; //GPIO Pin
    GPIO_TypeDef *port; //GPIO port
    buttonState_t prevButtonState; //previous button state
    buttonState_t currButtonState; //current button state
    int debounceCount; //number of current debounces
    int debounceThreshold; //debounce trigger
    int holdCount; //number of held cycles
    int holdThreshold; //button held trigger
    void (*pressedCb)(); //callback for unpressed state
    void (*longPressCb)(); //callback for long press state
} Button_t;


static  Button_t buttonArray[NUM_BUTTONS] = {
    [button0] = 
    {
        .pin = pushButton_Pin,
        .port = pushButton_GPIO_Port,
        .prevButtonState = invalidState,
        .currButtonState = invalidState,
        .debounceCount = 0,
        .debounceThreshold = DEBOUNCE_THRESHOLD,
        .holdCount = 0,
        .holdThreshold = HOLD_THRESHOLD,
        .pressedCb = &speedUp,
        .longPressCb = &led_test2
    }
};

void button_updateButton(void)
{
    int i;

    for(i = 0; i < NUM_BUTTONS; i++)
    {
        //if dbc < threshold and pin set
        if (HAL_GPIO_ReadPin(buttonArray[i].port, buttonArray[i].pin) == GPIO_PIN_SET)
        {
            if (buttonArray[i].debounceCount < buttonArray[i].debounceThreshold)
            {
                buttonArray[i].debounceCount++;
            }
        }
        //if dbc > 0 and pin is not set
        else if (HAL_GPIO_ReadPin(buttonArray[i].port, buttonArray[i].pin) == GPIO_PIN_RESET)
        {
            if (buttonArray[i].debounceCount > 0)
            {
                buttonArray[i].debounceCount--;
                buttonArray[i].holdCount = 0;
            }
        }

        //change states
        if (buttonArray[i].debounceCount == 0)
        {
            buttonArray[i].prevButtonState = buttonArray[i].currButtonState;
            buttonArray[i].currButtonState = unpressed;
        }
            //state is either pressed or long press
        else if (buttonArray[i].debounceCount == buttonArray[i].debounceThreshold)
        {
            //change to long press state
            if (buttonArray[i].holdCount == buttonArray[i].holdThreshold)
            {
                buttonArray[i].prevButtonState = buttonArray[i].currButtonState;
                buttonArray[i].currButtonState = longPress;
            }
            //stay in pressed state, increment hold count
            else
            {
                buttonArray[i].prevButtonState = buttonArray[i].currButtonState;
                buttonArray[i].currButtonState = pressed;
                buttonArray[i].holdCount++;
            }
        }
        //no valid state

        //***not necessary, since we want to stay in the current state even if 0 < dbc < 10
        // else
        // {
        //     buttonArray[i].prevButtonState = buttonArray[i].currButtonState;
        //     buttonArray[i].currButtonState = invalidState;
        //     break;
        // }
        
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
                    buttonArray[i].pressedCb();
                }
        }

    }
        
    return;
}