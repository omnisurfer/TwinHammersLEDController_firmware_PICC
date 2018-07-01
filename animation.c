/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "animation.h"
#include "ledPWMDriver.h"

/******************************************************************************/
/* Animation Routines                                                         */
/******************************************************************************/

/* typdef int to an animaiton function signature pointer to be returned by
 * AnimatonSelector
 * http://stackoverflow.com/questions/997821/how-to-make-a-function-return-a-pointer-to-a-function-c
 */

functionPointer AnimationSelector(AnimationList a)
{
    switch(a)
    {
        case Sweep:
            return SweepAnimation;

        case Fade:
            return FadeAnimation;

        case Fill:
            return FillAnimation;

        case Dominator:
            return DominatorAnimation;

        case Travel:
            return TravelAnimation;
            
        case AllOff:
            return AllOffAnimation;

        default:
            return AllOnAnimation;
    }
}

void SetFrameDelayTrigger(char delay, struct ANIMATION_RENDER_FRAME* aniRender_ptr)
{
    aniRender_ptr->frameDelayTrigger = delay;
}

/**
 * InitFrame proivdes a signle funciton that can be used to initialize the frame
 * the animation enters into the running state.
 * @param aniRender_ptr         pointer to animation function
 * @param pwmValue              PWM value that will be used to initialize the frame
 * @param frameDelay            Frame delay to be used initially
 * @param frameCount            Initial frame count, usually 0
 * @param frameDelayTrigger     Frame delay trigger
 */
void InitFrame(struct ANIMATION_RENDER_FRAME* aniRender_ptr, char pwmValue, char frameDelay, char frameCount, char frameDelayTrigger)
{
    for(int i = 0; i < 8; i++)
        aniRender_ptr->pwm[i] = pwmValue;

    aniRender_ptr->frameDelay = frameDelay;
    aniRender_ptr->frameCount = frameCount;
    aniRender_ptr->frameDelayTrigger = frameDelayTrigger;
}

void SweepAnimation(struct ANIMATION_RENDER_FRAME* aniRender_ptr)
{
    if(aniRender_ptr->animationState == 0)
    {
        InitFrame(aniRender_ptr, 0, 0, 0, 30);
        aniRender_ptr->animationState = 1;
    }

    int sweepAlpha = 0;
    
    char sweepValues[] = {MAX_BRIGHTNESS, MAX_BRIGHTNESS/4, MAX_BRIGHTNESS/32, 0, 0, 0, 0, 0 };

    //animation mode check - foreground or base
    if(aniRender_ptr->animationMode & 0x01)
    {
        sweepValues[3] = ALPHA_VALUE;
        sweepValues[4] = ALPHA_VALUE;
        sweepValues[5] = ALPHA_VALUE;
        sweepValues[6] = ALPHA_VALUE;
        sweepValues[7] = ALPHA_VALUE;
        sweepAlpha = ALPHA_VALUE;
    }
    
    if(aniRender_ptr->frameDelay == aniRender_ptr->frameDelayTrigger)
    {
        aniRender_ptr->frameDelay = 0;                

        if(aniRender_ptr->frameCount < 12)
        {
            //shift the PWM values, position 0 to position 7
            for(int i = 8; i > 0; i--)
            {
                aniRender_ptr->pwm[i] = aniRender_ptr->pwm[i-1];
            }

            if(aniRender_ptr->frameCount < 8)
                aniRender_ptr->pwm[0] = sweepValues[aniRender_ptr->frameCount];
            else
                aniRender_ptr->pwm[0] = sweepAlpha;

            aniRender_ptr->frameCount++;
        }
        
        //animation frames 12 through 32
        else if(aniRender_ptr->frameCount < 32)
        {
            //shift the PWM values, position 7 to position 0
            for(int i = 0; i < 8; i++)
            {
                aniRender_ptr->pwm[i] = aniRender_ptr->pwm[i+1];
            }

            if(aniRender_ptr->frameCount < 20)
                aniRender_ptr->pwm[7] = sweepValues[aniRender_ptr->frameCount - 12];
            else
                aniRender_ptr->pwm[7] = sweepAlpha;

            aniRender_ptr->frameCount++;
        }
        
        else
            aniRender_ptr->frameCount = 0;
    }

    else
        aniRender_ptr->frameDelay++;
}

void FadeAnimation(struct ANIMATION_RENDER_FRAME* aniRender_ptr)
{
    if(aniRender_ptr->animationState == 0)
    {        
        InitFrame(aniRender_ptr, 0, 0, 0, 30);
        aniRender_ptr->animationState = 1;       
    }
      
    if(aniRender_ptr->frameDelay == aniRender_ptr->frameDelayTrigger)
    {
        aniRender_ptr->frameDelay = 0;

        for(int i = 0; i < 8; i++)
        {
            if(aniRender_ptr->frameCount == 0)
                aniRender_ptr->pwm[i] = aniRender_ptr->pwm[i] += 1;
            else
                aniRender_ptr->pwm[i] = aniRender_ptr->pwm[i] -= 1;
        }

        if(aniRender_ptr->pwm[0] == MAX_BRIGHTNESS/4)
            aniRender_ptr->frameCount = 1;
        else if(aniRender_ptr->pwm[0] == 0)
            aniRender_ptr->frameCount = 0;
    }

    else
        aniRender_ptr->frameDelay++;
}

void TravelAnimation(struct ANIMATION_RENDER_FRAME* aniRender_ptr)
{
    if(aniRender_ptr->animationState == 0)
    {
        InitFrame(aniRender_ptr, 0, 0, 0, 127);
        aniRender_ptr->animationState = 1;
    }

    aniRender_ptr->frameDelay++;
    
    if(aniRender_ptr->frameDelay >= aniRender_ptr->frameDelayTrigger)
    {
        aniRender_ptr->frameDelay = 0;
        aniRender_ptr->frameCount++;
    }
    
    aniRender_ptr->pwm[aniRender_ptr->frameCount] = MAX_BRIGHTNESS;

    //clear the previous LED
    if(aniRender_ptr->frameCount > 0)
        aniRender_ptr->pwm[aniRender_ptr->frameCount - 1] = 0;
  
    if(aniRender_ptr->frameCount >= 8)
    {
        aniRender_ptr->pwm[aniRender_ptr->frameCount] = 0;
        aniRender_ptr->frameCount = 0;
    }
}

void FillAnimation(struct ANIMATION_RENDER_FRAME* aniRender_ptr)
{
    if(aniRender_ptr->animationState == 0)
    {
        InitFrame(aniRender_ptr, 0, 0, 0, 60);
        aniRender_ptr->animationState = 1;
    }

    if(aniRender_ptr->frameCount == 0)
        aniRender_ptr->pwm[0] = MAX_BRIGHTNESS/16;

    if(aniRender_ptr->frameCount == 1)
        aniRender_ptr->pwm[1] = MAX_BRIGHTNESS/16;

    if(aniRender_ptr->frameCount == 2)
        aniRender_ptr->pwm[2] = MAX_BRIGHTNESS/16;

    if(aniRender_ptr->frameCount == 3)
        aniRender_ptr->pwm[3] = MAX_BRIGHTNESS/16;

    if(aniRender_ptr->frameCount == 4)
        aniRender_ptr->pwm[4] = MAX_BRIGHTNESS/16;

    if(aniRender_ptr->frameCount == 5)
        aniRender_ptr->pwm[5] = MAX_BRIGHTNESS/16;

    if(aniRender_ptr->frameCount == 6)
        aniRender_ptr->pwm[6] = MAX_BRIGHTNESS/16;

    if(aniRender_ptr->frameCount == 7)
        aniRender_ptr->pwm[7] = MAX_BRIGHTNESS/16;

    if(aniRender_ptr->frameCount >= 15)
    {
        aniRender_ptr->frameCount = 0;

        aniRender_ptr->pwm[0] = 0;
        aniRender_ptr->pwm[1] = 0;
        aniRender_ptr->pwm[2] = 0;
        aniRender_ptr->pwm[3] = 0;
        
        aniRender_ptr->pwm[4] = 0;
        aniRender_ptr->pwm[5] = 0;
        aniRender_ptr->pwm[6] = 0;
        aniRender_ptr->pwm[7] = 0;
    }

    else if(aniRender_ptr->frameDelay == aniRender_ptr->frameDelayTrigger)
    {
        aniRender_ptr->frameCount++;
        aniRender_ptr->frameDelay = 0;
    }

    else
        aniRender_ptr->frameDelay++;
}

void DominatorAnimation(struct ANIMATION_RENDER_FRAME* aniRender_ptr)
{
    if(aniRender_ptr->animationState == 0)
    {
        InitFrame(aniRender_ptr, 0, 0, 0, 30);
        aniRender_ptr->animationState = 1;
    }

    //initialize the frame buffer with base of dominator animation
    char dominatorBase[] = {MAX_BRIGHTNESS/64, MAX_BRIGHTNESS/64, MAX_BRIGHTNESS/16, MAX_BRIGHTNESS/4, MAX_BRIGHTNESS/4, MAX_BRIGHTNESS/16, MAX_BRIGHTNESS/64, MAX_BRIGHTNESS/64};
        
    if(aniRender_ptr->frameDelay == aniRender_ptr->frameDelayTrigger)
    {
        //copy to pwm buffer of animation render

        aniRender_ptr->pwm[0] = dominatorBase[0];
        aniRender_ptr->pwm[1] = dominatorBase[1];
        aniRender_ptr->pwm[2] = dominatorBase[2];
        aniRender_ptr->pwm[3] = dominatorBase[3];

        aniRender_ptr->pwm[4] = dominatorBase[4];
        aniRender_ptr->pwm[5] = dominatorBase[5];
        aniRender_ptr->pwm[6] = dominatorBase[6];
        aniRender_ptr->pwm[7] = dominatorBase[7];

        if(aniRender_ptr->frameCount == 0)
        {            
            aniRender_ptr->pwm[3] = MAX_BRIGHTNESS/2;
            aniRender_ptr->pwm[4] = MAX_BRIGHTNESS/2;
        }

        else if(aniRender_ptr->frameCount == 1)
        {
            aniRender_ptr->pwm[2] = MAX_BRIGHTNESS/2;
            //aniRender_ptr->pwm[3] = MAX_BRIGHTNESS/16;
            //aniRender_ptr->pwm[4] = MAX_BRIGHTNESS/16;
            aniRender_ptr->pwm[5] = MAX_BRIGHTNESS/2;
        }

        else if(aniRender_ptr->frameCount == 2)
        {
            aniRender_ptr->pwm[1] = MAX_BRIGHTNESS/2;
            aniRender_ptr->pwm[2] = MAX_BRIGHTNESS/8;
            aniRender_ptr->pwm[3] = MAX_BRIGHTNESS/16;
            aniRender_ptr->pwm[4] = MAX_BRIGHTNESS/16;
            aniRender_ptr->pwm[5] = MAX_BRIGHTNESS/8;
            aniRender_ptr->pwm[6] = MAX_BRIGHTNESS/2;
        }
        
        else if(aniRender_ptr->frameCount == 3)
        {
            aniRender_ptr->pwm[0] = MAX_BRIGHTNESS/2;
            aniRender_ptr->pwm[1] = MAX_BRIGHTNESS/8;
            aniRender_ptr->pwm[2] = MAX_BRIGHTNESS/32;
            aniRender_ptr->pwm[3] = MAX_BRIGHTNESS/8;
            aniRender_ptr->pwm[4] = MAX_BRIGHTNESS/8;
            aniRender_ptr->pwm[5] = MAX_BRIGHTNESS/32;
            aniRender_ptr->pwm[6] = MAX_BRIGHTNESS/8;
            aniRender_ptr->pwm[7] = MAX_BRIGHTNESS/2;
        }

        else if(aniRender_ptr->frameCount == 4)
        {
            aniRender_ptr->pwm[0] = MAX_BRIGHTNESS/8;
            aniRender_ptr->pwm[1] = MAX_BRIGHTNESS/32;
            //aniRender_ptr->pwm[2] = MAX_BRIGHTNESS/8;
            //aniRender_ptr->pwm[3] = MAX_BRIGHTNESS/4;
            //aniRender_ptr->pwm[4] = MAX_BRIGHTNESS/4;
            //aniRender_ptr->pwm[5] = MAX_BRIGHTNESS/8;
            aniRender_ptr->pwm[6] = MAX_BRIGHTNESS/32;
            aniRender_ptr->pwm[7] = MAX_BRIGHTNESS/8;
        }
        
        else if(aniRender_ptr->frameCount == 5)
        {
            aniRender_ptr->pwm[0] = MAX_BRIGHTNESS/32;
            aniRender_ptr->pwm[7] = MAX_BRIGHTNESS/32;
            //while(true);
        }

        else
        {
            //do nothing
        }
                
        aniRender_ptr->frameCount++;
        aniRender_ptr->frameDelay = 0;

        if(aniRender_ptr->frameCount > 6)
            aniRender_ptr->frameCount = 0;
    }

    else
        aniRender_ptr->frameDelay++;    
}

void AllOnAnimation(struct ANIMATION_RENDER_FRAME* aniRender_ptr)
{
    if(aniRender_ptr->animationState == 0)
    {
        InitFrame(aniRender_ptr, 0, 0, 0, 35);
        aniRender_ptr->animationState = 1;
    }

    if(aniRender_ptr->animationMode == 0x10)
    {
        for(int i = 0; i < 8; i++)
        {
            aniRender_ptr->pwm[i] = MAX_BRIGHTNESS;
        }
    }

    else
    {
        for(int i = 0; i < 8; i++)
        {
            aniRender_ptr->pwm[i] = MAX_BRIGHTNESS/2;
        }
    }
}

void AllOffAnimation(struct ANIMATION_RENDER_FRAME* aniRender_ptr)
{
    if(aniRender_ptr->animationState == 0)
    {
        InitFrame(aniRender_ptr, 0, 0, 0, 35);
        aniRender_ptr->animationState = 1;
    }

    int sweepAlpha = 0;

    if(aniRender_ptr->animationMode & 0x01)
    {
        sweepAlpha = ALPHA_VALUE;
    }
        
    aniRender_ptr->pwm[0] = sweepAlpha;
    aniRender_ptr->pwm[1] = sweepAlpha;
    aniRender_ptr->pwm[2] = sweepAlpha;
    aniRender_ptr->pwm[3] = sweepAlpha;

    aniRender_ptr->pwm[4] = sweepAlpha;
    aniRender_ptr->pwm[5] = sweepAlpha;
    aniRender_ptr->pwm[6] = sweepAlpha;
    aniRender_ptr->pwm[7] = sweepAlpha;
}