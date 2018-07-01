/* 
 * File:   render.h
 * Author: Daniel
 *
 * Created on February 9, 2014, 3:52 PM
 */

#ifndef RENDER_H
#define	RENDER_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* RENDER_H */

/******************************************************************************/
/* Render #define Macros                                                   */
/******************************************************************************/

//#define A                 1
//#define B                 2
//#define C                 3

/******************************************************************************/
/* Render Function Variables                                               */
/******************************************************************************/

/******************************************************************************/
/* Render Function Prototypes                                              */
/******************************************************************************/

/**
 * Function that takes Animation Frame structs and renders them to the LED buffer
 * @param animationFunctionArray
 * @param animationFrameArray
 * @param timerTicks
 */
void RenderAnimation(
        void (*animationFunctionArray[])(struct ANIMATION_RENDER_FRAME*),
        struct ANIMATION_RENDER_FRAME* animationFrameArray[],
        struct SYSTEM_TIMERS* timerTicks
    ); 

/**
 * 
 * @param aniBackgroundFrame_ptr
 * @param aniForegroundFrame_ptr
 * @param aniOutputBuffer_ptr
 */
void AnimationCompositing(
        struct ANIMATION_RENDER_FRAME* aniBackgroundFrame_ptr,
        struct ANIMATION_RENDER_FRAME* aniForegroundFrame_ptr,
        struct ANIMATION_OUTPUT_BUFFER* aniOutputBuffer_ptr
    );

/**
 * 
 * @param baseAnimation
 * @param baseFrame_ptr
 * @param baseAnimationState
 * @param baseAnimationMode
 * @param foregroundAnimation
 * @param foregroundFrame_ptr
 * @param foregoundAnimationState
 * @param foregroundAnimationMode
 * @return 
 */
int SetAnimationQue(
        AnimationList baseAnimation,
        struct ANIMATION_RENDER_FRAME* baseFrame_ptr,
        char baseAnimationState,
        char baseAnimationMode,
        AnimationList foregroundAnimation,
        struct ANIMATION_RENDER_FRAME* foregroundFrame_ptr,
        char foregoundAnimationState,
        char foregroundAnimationMode
    );