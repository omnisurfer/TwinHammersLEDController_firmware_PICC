/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "processor_common.h"
#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
#include "timing.h"
#include "port_common.h"
#include "ledPWMDriver.h"
#include "animation.h"
#include "buttons.h"

#include "uart.h"
#include "render.h"


/******************************************************************************/
/* User Global Variable Declaration
 * Not fond of global but makes things easier for now
/******************************************************************************/
/* <Initialize variables in user.h and insert code for user algorithms.> */


/******************************************************************************/
/* Main Program
 * Four clock edges per instruction. At 8MHz FOSC, one instruction takes
 * 500ns to execute (2MIPS). At 20MHz FOSC, 200ns (5MIPS)
/******************************************************************************/

/*SVN test after server IP change*/

/*
 * TODO:
 * DMR - 2014-03-10: Re-consider changing ints to char since ints take up 16-bits which is likely emulated in software.
 * DMR - 2014-06-14:
 *  W Create a HAL so that code can be transported to different uC architectures. Remove PIC specific code from as many functions as possible and collect them into the HAL.
 *  X Make animationLoopTrigger represent an actual time (i.e. every count is 1ms) instead of a dimensionless number.
 *  X Create an "alpha" value in the PWM that is not "0". May make this higher than the maximum PWM duty count of 127 (i.e. 128 equals alpha).
 *  - Refine animations so they can be re-used. For example, create a variable that can be passed to allow the sweep direction to be changed without having to hard code.
 *  - Create interface that allows the frame to be sent over UART (i.e. from a PC).
 *  W Find a way to programatically switch in which effects get rendered instead of current hard coding method.
 *  X Simplify RenderAnimationHousekeeping so it does not require start and end routines or do not use them externally (make them apart of typical rendering calls).
 * DMR - 2014-07-07
 *  W Get rid of magic numbers like PWMdimmersteps being manually entered as 128
 */

//Dev Reference
//http://microchip.wikidot.com/

int test = 0;

void main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();

    InitializeUART();
      
    struct ANIMATION_RENDER_FRAME backgroundFrame =
    {       
        {0},
        {0},
        {0},
        {0},
        {0},
        {0, 0, 0, 0, 0, 0, 0, 0},
    };

    struct ANIMATION_RENDER_FRAME foregroundFrame =
    {
        {0},
        {0},
        {0},
        {0},
        {0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };

    struct ANIMATION_OUTPUT_BUFFER outputBuffer =
    {        
        {0, 0, 0, 0, 0, 0, 0, 0}
    };
    
    struct SYSTEM_TIMERS timerTicks =
    {        
        {0},
        {0},
        {0},
        {0},        

        {0},
        {0},
        {0},
        {0}        
    };        
    
    char debugTrigger = 0, selectAnimation = 0, mainButtonState = 0;    

    //read the switch every ms
    bool buttonOK = false;

    //create a function pointer array to hold the addresses of the animations
    void (*animationFunctionArray[7])(struct ANIMATION_RENDER_FRAME*);

    animationFunctionArray[0] = AnimationSelector(Sweep);
    animationFunctionArray[1] = AnimationSelector(Fade);
    animationFunctionArray[2] = AnimationSelector(Fill);
    animationFunctionArray[3] = AnimationSelector(Travel);
    animationFunctionArray[4] = AnimationSelector(Dominator);
    animationFunctionArray[5] = AnimationSelector(AllOn);
    animationFunctionArray[6] = AnimationSelector(AllOff);

    //char functionArraySize = sizeof(animationFunctionArray) / sizeof(animationFunctionArray[0]);
    
    //create an array to hold the animation structs addresses
    struct ANIMATION_RENDER_FRAME* animationFrameArray[2];

    animationFrameArray[0] = &backgroundFrame;
    animationFrameArray[1] = &foregroundFrame;

    //Set the initial animation que;
    SetAnimationQue(AllOff, animationFrameArray[0], 0, 0, AllOff, animationFrameArray[1], 0, 0);
        
    //main execution loop
    //107uS - 148uS
    while(1)
    {                
        //
        //animation - currently three animations worth of structs leave about ~30% Data Memory left over
        //                                          
         
        /* Rendering to follow this pattern:
         * Render Layer 0 (Base - i.e. All LEDs fade effect)
         * Render Layer 1 (Top with alpha - i.e. Sweep effect with some LED's "OFF"
         * Draw the Output of the combined two layers         
         */       
        //16uS - 68uS; 132uS with two animations
        RenderAnimation(animationFunctionArray, animationFrameArray, &timerTicks);
                
        //42uS
        WritePin(PORT_A, PIN_2, true);

        AnimationCompositing(&backgroundFrame, &foregroundFrame, &outputBuffer);

        WritePin(PORT_A, PIN_2, false);
        
        //take animation/frame and write it out to the LEDs
        //38uS               
        WritePort(PORT_C, ProcessLEDPWM(&outputBuffer));
        
        //24uS
        TimerDaemon(&timerTicks);        

        //16uS
        if(timerTicks.baseClock_msCounter == 50)
            buttonOK = debouncedSwitch(PORT_A, PIN_3, &mainButtonState);
                
        if(buttonOK)
        {                                                                        
            if(selectAnimation > 8)
                selectAnimation = 0;

            switch(selectAnimation)
            {
                case 0:
                    SetAnimationQue(Sweep, animationFrameArray[0], 0, animationFrameArray[0]->animationMode | 0x00,
                                    AllOff, animationFrameArray[1], 0, animationFrameArray[1]->animationMode | 0x01);
                    break;

                case 1:
                    SetAnimationQue(Fade, animationFrameArray[0], 0, animationFrameArray[0]->animationMode | 0x00,
                                    AllOff, animationFrameArray[1], 0, animationFrameArray[1]->animationMode | 0x01);
                    break;

                case 2:
                    SetAnimationQue(Fill, animationFrameArray[0], 0, animationFrameArray[0]->animationMode | 0x00,
                                    AllOff, animationFrameArray[1], 0, animationFrameArray[1]->animationMode | 0x01);
                    break;

                case 3:
                    SetAnimationQue(Travel, animationFrameArray[0], 0, animationFrameArray[0]->animationMode | 0x00,
                                    AllOff, animationFrameArray[1], 0, animationFrameArray[1]->animationMode | 0x01);
                    break;

                case 4:
                    SetAnimationQue(Dominator, animationFrameArray[0], 0, animationFrameArray[0]->animationMode | 0x00,
                                AllOff, animationFrameArray[1], 0, animationFrameArray[1]->animationMode | 0x01);
                    break;

                case 5:
                    SetAnimationQue(AllOn, animationFrameArray[0], 0, animationFrameArray[0]->animationMode | 0x10,
                                    AllOff, animationFrameArray[1], 0, animationFrameArray[1]->animationMode | 0x01);
                    break;

                case 6:
                    SetAnimationQue(AllOn, animationFrameArray[0], 0, animationFrameArray[0]->animationMode & 0xEF,
                                    AllOff, animationFrameArray[1], 0, animationFrameArray[1]->animationMode | 0x01);
                    break;

                case 7:
                    SetAnimationQue(Fade, animationFrameArray[0], 0, animationFrameArray[0]->animationMode | 0x00,
                                    Travel, animationFrameArray[1], 0, animationFrameArray[1]->animationMode | 0x01);
                    break;

                case 8:
                    SetAnimationQue(AllOff, animationFrameArray[0], 0, animationFrameArray[0]->animationMode | 0x00,
                                    AllOff, animationFrameArray[1], 0, animationFrameArray[1]->animationMode | 0x01);
                    break;

                default:
                    selectAnimation = 0;
                    break;
            }

            selectAnimation++;
        }

        //
        //send out a test UART transmission - only when no transmission is occurring
        //This will be implemented better but wanted an easy way to test the UART without bogging
        //the rest of the micro down.
        //       
        
        if((&timerTicks)->baseClock_1000msTick == debugTrigger)
        {
            if(TXSTAbits.TRMT == 1)
                UARTTransmit(0x41);
            debugTrigger = !debugTrigger;
        }       
    }
}

