/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "animation.h"
#include "timing.h"
#include "render.h"
#include "ledPWMDriver.h"

/******************************************************************************/
/* Render Function Variables                                               */
/******************************************************************************/

/*bools used to interface with timerDeamon*/
//bool _currentEdge = false, _previousEdge = false;

char renderLoopCounter = 0;
char renderLoopTrigger = 16; //33ms Tick, about ~30fps

AnimationList backgroundLayer, foregroundLayer;

/******************************************************************************/
/* Render Routines                                                         */
/******************************************************************************/

//Change this so it can takes a function pointer:
//http://www.cprogramming.com/tutorial/function-pointers.html
/**
 * RenderAnimation -    Takes in an animation function pointer and increments the animation by a single frame
 *                      May one day change this to take an array of animation function pointers so I can increment through them.
 *                      This could save some more space
 *
 *
 */
void RenderAnimation(void (*animationFunctionArray[])(struct ANIMATION_RENDER_FRAME*),
        struct ANIMATION_RENDER_FRAME* animationFrameArray[],
        struct SYSTEM_TIMERS* timerTicks
    )
{
    if(timerTicks->baseClock_msTick)
        renderLoopCounter++;

    if(renderLoopCounter == renderLoopTrigger)
    {
        (*animationFunctionArray[backgroundLayer])(animationFrameArray[0]);
        (*animationFunctionArray[foregroundLayer])(animationFrameArray[1]);
        
        renderLoopCounter = 0;
    }
}

void AnimationCompositing(struct ANIMATION_RENDER_FRAME* aniBackgroundFrame_ptr,
    struct ANIMATION_RENDER_FRAME* aniForegroundFrame_ptr,
    struct ANIMATION_OUTPUT_BUFFER* aniOutputBuffer_ptr
    )
{ 
    /*Loop unrolled for speed gain*/
    /*
    for(int i = 0; i < 8; i++)
    {
        if(aniForegroundFrame_ptr->pwm[i] != ALPHA_VALUE)
            aniOutputBuffer_ptr->pwm[i] = aniForegroundFrame_ptr->pwm[i];

        else
            aniOutputBuffer_ptr->pwm[i] = aniBackgroundFrame_ptr->pwm[i];
    }
    */
    //fill outputBuffer values
    aniOutputBuffer_ptr->pwm[0] = aniBackgroundFrame_ptr->pwm[0];
    aniOutputBuffer_ptr->pwm[1] = aniBackgroundFrame_ptr->pwm[1];
    aniOutputBuffer_ptr->pwm[2] = aniBackgroundFrame_ptr->pwm[2];
    aniOutputBuffer_ptr->pwm[3] = aniBackgroundFrame_ptr->pwm[3];
    aniOutputBuffer_ptr->pwm[4] = aniBackgroundFrame_ptr->pwm[4];
    aniOutputBuffer_ptr->pwm[5] = aniBackgroundFrame_ptr->pwm[5];
    aniOutputBuffer_ptr->pwm[6] = aniBackgroundFrame_ptr->pwm[6];
    aniOutputBuffer_ptr->pwm[7] = aniBackgroundFrame_ptr->pwm[7];    
    
    //selectively insert base layer based on alpha in layer 0
    if(aniForegroundFrame_ptr->pwm[0] != ALPHA_VALUE)
        aniOutputBuffer_ptr->pwm[0] = aniForegroundFrame_ptr->pwm[0];

    if(aniForegroundFrame_ptr->pwm[1] != ALPHA_VALUE)
        aniOutputBuffer_ptr->pwm[1] = aniForegroundFrame_ptr->pwm[1];

    if(aniForegroundFrame_ptr->pwm[2] != ALPHA_VALUE)
        aniOutputBuffer_ptr->pwm[2] = aniForegroundFrame_ptr->pwm[2];
    
    if(aniForegroundFrame_ptr->pwm[3] != ALPHA_VALUE)
        aniOutputBuffer_ptr->pwm[3] = aniForegroundFrame_ptr->pwm[3];

    if(aniForegroundFrame_ptr->pwm[4] != ALPHA_VALUE)
        aniOutputBuffer_ptr->pwm[4] = aniForegroundFrame_ptr->pwm[4];

    if(aniForegroundFrame_ptr->pwm[5] != ALPHA_VALUE)
        aniOutputBuffer_ptr->pwm[5] = aniForegroundFrame_ptr->pwm[5];

    if(aniForegroundFrame_ptr->pwm[6] != ALPHA_VALUE)
        aniOutputBuffer_ptr->pwm[6] = aniForegroundFrame_ptr->pwm[6];

    if(aniForegroundFrame_ptr->pwm[7] != ALPHA_VALUE)
        aniOutputBuffer_ptr->pwm[7] = aniForegroundFrame_ptr->pwm[7];
}

/**
 * 
 * @param baseAnimation
 * @param baseFrame_ptr
 * @param baseAnimationState
 * @param baseAnimationMode
 * @param firstAnimation
 * @param firstFrame_ptr
 * @param firstAnimationState
 * @param baseAnimationMode
 * @return
 */
int SetAnimationQue(
        AnimationList baseAnimation,
        struct ANIMATION_RENDER_FRAME* backgroundFrame_ptr,
        char backgroundAnimationState,
        char backgroundAnimationMode,
        AnimationList foregroundAnimation,
        struct ANIMATION_RENDER_FRAME* foregroundFrame_ptr,
        char foregoundAnimationState,
        char foregroundAnimationMode
    )
{    
    backgroundFrame_ptr->animationState = backgroundAnimationState;
    foregroundFrame_ptr->animationState = foregoundAnimationState;

    backgroundFrame_ptr->animationMode = backgroundAnimationMode;
    foregroundFrame_ptr->animationMode = foregroundAnimationMode;

    backgroundLayer = baseAnimation;
    foregroundLayer = foregroundAnimation;
    return 0;
}