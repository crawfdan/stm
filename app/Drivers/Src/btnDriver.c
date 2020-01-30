#include "btnDriver.h"

static  Button buttonArray[NUM_BTNS];

void updateBtn(void)
{
    int i;

    for(i = 0; i < sizeof(buttonArray); i++)
    {
        //if the button is pressed and current state is pressed
        if (buttonArray[i].pinVal == GPIO_PIN_SET &&
        buttonArray[i].btnState == pressed)
        {
            buttonArray[i].holdCount++;
        }
        //if the button is not being pressed
        else if(buttonArray[i].pinVal == GPIO_PIN_RESET &&
        buttonArray[i].btnState == notPressed)
        {
            buttonArray[i].debounceCount++;
        }

        //update states

        //unpressed -> pressed
        if (buttonArray[i].debounceCount == buttonArray[i].debounceThreshold)
        {
            buttonArray[i].prevButtonState = buttonArray[i].btnState;
            buttonArray[i].btnState = pressed;
            buttonArray[i].pressedCallback();
        }

        //pressed -> long press
        else if (buttonArray[i].holdCount == buttonArray[i].holdThreshold)
        {
            buttonArray[i].prevButtonState = buttonArray[i].btnState;
            buttonArray[i].btnState = longPress;
            buttonArray[i].longPressCallback();
        }

        //pressed -> unpressed
        else if (buttonArray[i].btnState == pressed && 
        buttonArray[i].pinVal == GPIO_PIN_RESET)
        {
            buttonArray[i].prevButtonState = buttonArray[i].btnState;
            buttonArray[i].btnState = notPressed;
            buttonArray[i].unpressedCallback();
        }

        //long press -> unpressed
        else if (buttonArray[i].btnState == longPress && 
        buttonArray[i].pinVal == GPIO_PIN_RESET)
        {
            buttonArray[i].prevButtonState = buttonArray[i].btnState;
            buttonArray[i].btnState = notPressed;
            buttonArray[i].unpressedCallback();
        }

        else
        {
            /* code */
        }
        
    return;
}