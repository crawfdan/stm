#include "led.h"
#include "main.h"
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "stm32f207xx.h"
#include "FreeRTOS.h"


#define NUM_LEDS 3
#define NUM_COLLS 1
#define BLINK_THRESHOLD 100

typedef enum {
    ledRGB_Red = 0,
    ledRGB_Green,
    ledRGB_Blue,
} ledRGB_t;


//current given behavior for led collection
typedef enum {
    ledBehavior_Drive = 0x0,
    ledBehavior_Blinking 
} ledBehavior_t;

//current state of the led collection
typedef enum {
    ledCollectionState_Off = 0x0,
    ledCollectionState_On
} ledCollectionState_t;

//single led structure
typedef struct led {
    uint16_t pin; //GPIO Pin of led
    GPIO_TypeDef *port; //GPIO port of led
} led_t;

//data stored for blinking led collection
typedef struct blinkInfo {
    long blinkCount; //current number of cycles to blink (duration)
    long blinkThreshold; //max number of cycles to blink
    int numBlinks; //how many times the led will blink
} blinkInfo_t;

//struct for collection of rgb leds
struct collection {
    led_t leds[NUM_LEDS]; //leds in the collection
    ledColors_t color; //current color of collection
    blinkInfo_t blink; //blink info for collection
    ledCollectionState_t state; //current on/off state of collection
    ledBehavior_t behavior; //driving or blinking
};


static Collection_t collArray[NUM_COLLS] = {
    [ledCollections_1] =
    {
        .leds = {
                    [ledRGB_Red] = 
                    {
                        .pin = ledR_Pin,
                        .port = ledR_GPIO_Port
                    },

                    [ledRGB_Green] = 
                    {
                        .pin = ledG_Pin,
                        .port = ledG_GPIO_Port
                    },

                    [ledRGB_Blue] = 
                    {
                        .pin = ledB_Pin,
                        .port = ledB_GPIO_Port
                    }
                },
        .color = ledColors_Blue,
        .blink = {.blinkCount = 0, .blinkThreshold = BLINK_THRESHOLD, .numBlinks = 10},
        .state = ledCollectionState_Off,
        .behavior = ledBehavior_Drive
    }
};


//turn on the led indefinitely
void led_On(ledCollections_t index)
{
    Collection_t *coll = &collArray[index];
    int i;
    unsigned mask = 0x1;
    for(i = 0; i < NUM_LEDS; i++)
    {
        //if there is a 1 in the bit position of the color, turn on that pin
        if ((coll->color & mask) > 0)
        {
            HAL_GPIO_WritePin(coll->leds[i].port, coll->leds[i].pin, SET);
        }

        //shift mask for next bit in color
        mask <<= 1;
    }
    coll->state = ledCollectionState_On;
    return;
}

//turns off leds in a collection indefinitely
void led_Off (ledCollections_t index)
{
    Collection_t *coll = &collArray[index];
    int i;
    //set all leds to off state
    for(i = 0; i < NUM_LEDS; i++)
    {
        HAL_GPIO_WritePin(coll->leds[i].port, coll->leds[i].pin, RESET);
    }
    //update states
    coll->state = ledCollectionState_Off;
    return;
}

void led_ColorChange (ledCollections_t index, ledColors_t newColor)
{
    assert(newColor >= ledColors_Red);

    Collection_t *coll = &collArray[index];
    coll->color = newColor;
    return;
}

//toggle led state or increase blink counter
bool led_Blink (ledCollections_t index)
{
    //asserts
    // assert(coll.blink.numBlinks >= 0);
    // assert(coll.behavior == ledBehavior_Blinking);--

    Collection_t *coll = &collArray[index];
    //if number of blinks satisfied, return to task
    if (coll->blink.numBlinks == 0)
    {
        led_StopBlink(index);
        return false;
    }
    //if we meet the threshold, toggle the pin
    else if (coll->blink.blinkCount == coll->blink.blinkThreshold)
    {
        if (coll->state == ledCollectionState_Off)
        {
            led_On(index);
            //coll->blink.numBlinks--;
        }

        else
        {
            led_Off(index);
        }
        coll->blink.blinkCount = 0;
    }

    else
    {
        coll->blink.blinkCount++;
    }

    return true;
}

//stop led from blinking and update behavior
void led_StopBlink (ledCollections_t index)
{
    Collection_t *coll = &collArray[index];
    led_Off(index);
    coll->behavior = ledBehavior_Drive;
    return;
}

//create a task for led handling
void led_taskCreate()
{
    xTaskCreate();
    return;
}

/***************************************************************
****************************************************************
TEST FUNCTIONS
*****************************************************************
*****************************************************************/

//change color to red and turn on leds
void led_activateRedLed()
{
    led_ColorChange(ledCollections_1, ledColors_Red);
    led_On(ledCollections_1);
    return;
}

//change color to blue and turn on leds
void led_activateBlueLed()
{
    //collArray[ledCollections_1].color = ledColors_Blue;
    led_ColorChange(ledCollections_1, ledColors_Blue);
    led_On(ledCollections_1);
    return;
}

//change color to green and turn on leds
void led_activateGreenLed()
{
    led_ColorChange(ledCollections_1, ledColors_Green);
    led_On(ledCollections_1);
    return;
}

void led_test()
{
    led_On(ledCollections_1);
    return;
}
void led_test2()
{
    //led_Off(ledCollections_1);
    // collArray[ledCollections_1].color = ledColors_Red;
    led_ColorChange(ledCollections_1, nextColor());
    return;
}

bool blink()
{
    bool blinking = true;
    collArray[ledCollections_1].behavior = ledBehavior_Blinking;
    blinking = led_Blink(ledCollections_1);
    return blinking;
}

ledColors_t nextColor()
{
    if (collArray[ledCollections_1].color == ledColors_White)
    {
        collArray[ledCollections_1].color = ledColors_Red;
        return ledColors_White;
    }

    else
    {
        return collArray[ledCollections_1].color++;
    }
    
}

void speedUp()
{
    if(collArray[ledCollections_1].blink.blinkThreshold > 10)
    {
        collArray[ledCollections_1].blink.blinkThreshold -= 10;
    }
    
    return;
}

// void blinkColors()
// {
//     while (blink())
//     {
//         return;
//     }

//     collArray[ledCollections_1].color = nextColor();
//     collArray[ledCollections_1].blink.numBlinks = 10;
//     return;
    
// }