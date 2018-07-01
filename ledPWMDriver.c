/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "port_common.h"
#include "ledPWMDriver.h"
#include "animation.h"

/******************************************************************************/
/* Draw Function Variables                                               */
/******************************************************************************/

char brightnessTemp = 0, ledBuffer = 0x00;

/******************************************************************************/
/* ledDriver Routines                                                              */
/******************************************************************************/

/**
 * ProcessLEDPWM            - takes in a pointer to the PORT to animate to and a pointer to the next animation frame to render onto the LEDs on that port
 * @param aniRender_ptr     - Pointer to the animation frame buffer and struct
 */

char ProcessLEDPWM(struct ANIMATION_OUTPUT_BUFFER* aniOutputBuffer_ptr)
{    
    if(brightnessTemp == MAX_BRIGHTNESS)
    {
        brightnessTemp = 0;           
        ledBuffer = 0xFF;
    }
        
    //blank if brightnessTemp is greater than PWM value

    //LED7
    if(brightnessTemp >= aniOutputBuffer_ptr->pwm[7])
        ledBuffer = ledBuffer & 0x7F;
       
    //LED6
    if(brightnessTemp >= aniOutputBuffer_ptr->pwm[6])
        ledBuffer = ledBuffer & 0xBF;    
   
    //LED5
    if(brightnessTemp >= aniOutputBuffer_ptr->pwm[5])
        ledBuffer = ledBuffer & 0xDF;    
    
    //LED4
    if(brightnessTemp >= aniOutputBuffer_ptr->pwm[4])
        ledBuffer = ledBuffer & 0xEF;    
    
    //LED3
    if(brightnessTemp >= aniOutputBuffer_ptr->pwm[3])
        ledBuffer = ledBuffer & 0xF7;    

    //LED2
    if(brightnessTemp >= aniOutputBuffer_ptr->pwm[2])
        ledBuffer = ledBuffer & 0xFB;    
    
    //LED1
    if(brightnessTemp >= aniOutputBuffer_ptr->pwm[1])
        ledBuffer = ledBuffer & 0xFD;    
    
    //LED0
    if(brightnessTemp >= aniOutputBuffer_ptr->pwm[0])
        ledBuffer = ledBuffer & 0xFE;    
        
    brightnessTemp++;   
    
    return ledBuffer;
}