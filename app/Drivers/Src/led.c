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

typedef enum {
    invalidCollection = -1,
    collection1
} ledCollections_t;

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
typedef struct collection {
    led_t leds[NUM_LEDS]; //leds in the collection
    ledColors_t color; //current color of collection
    blinkInfo_t blink; //blink info for collection
    ledCollectionState_t state; //current on/off state of collection
    ledBehavior_t behavior; //driving or blinking
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
                        .pin = ledR_Pin,
                        .port = ledR_GPIO_Port
                    },

                    [1] = 
                    {
                        .pin = ledG_Pin,
                        .port = ledG_GPIO_Port
                    },

                    [2] = 
                    {
                        .pin = ledB_Pin,
                        .port = ledB_GPIO_Port
                    }
                },
        .color = ledColors_White,
        .blink = {.blinkCount = 0, .blinkThreshold = BLINK_THRESHOLD, .numBlinks = 10},
        .state = ledCollectionState_Off,
        .behavior = ledBehavior_Drive
    }
};


//turn on the led indefinitely
void led_On(Collection_t coll)
{
    int i;
    unsigned mask = 0x1;
    for(i = 0; i < NUM_LEDS; i++)
    {
        //if there is a 1 in the bit position of the color, turn on that pin
        if ((coll.color & mask) > 0)
        {
            HAL_GPIO_WritePin(coll.leds[i].port, coll.leds[i].pin, SET);
        }

        //shift mask for next bit in color
        mask <<= 1;
    }
    coll.state = ledCollectionState_On;
    return;
}

//turns off leds in a collection indefinitely
void led_Off (Collection_t coll)
{
    int i;
    //set all leds to off state
    for(i = 0; i < NUM_LEDS; i++)
    {
        HAL_GPIO_WritePin(coll.leds[i].port, coll.leds[i].pin, RESET);
    }
    //update states
    coll.state = ledCollectionState_Off;
    return;
}

void led_ColorChange (Collection_t coll, ledColors_t newColor)
{
    assert(newColor >= ledColors_Red);
    coll.color = newColor;
    return;
}

void led_Blink (Collection_t coll)
{
    //if we meet the threshold, toggle the pin
    if (coll.blink.blinkCount == coll.blink.blinkThreshold)
    {
        if (coll.state == ledCollectionState_Off)
        {
            led_On(coll);
            coll.blink.numBlinks++;
        }

        else
        {
            led_Off(coll);
        }
        
    }

    else
    {
        coll.blink.blinkCount++;
    }

    return;
}

void led_activateRedLed()
{
    led_ColorChange(collArray[collection1], ledColors_Red);
    led_On(collArray[collection1]);
    return;
}

void led_activateBlueLed()
{
    led_ColorChange(collArray[collection1], ledColors_Blue);
    led_On(collArray[collection1]);
    return;
}

void led_activateGreenLed()
{
    led_ColorChange(collArray[collection1], ledColors_Green);
    led_On(collArray[collection1]);
    return;
}