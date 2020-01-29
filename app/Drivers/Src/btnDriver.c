#include "btnDriver.h"

void updateBtn(void)
{
    int i;
    Button buttonArray[NUM_BTNS];

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

        
    }

    return;
}