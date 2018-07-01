/* 
 * File:   animation.h
 * Author: Daniel
 *
 * Created on February 9, 2014, 3:52 PM
 */

#ifndef ANIMATION_H
#define	ANIMATION_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* ANIMATION_H */

/******************************************************************************/
/* Animation #define Macros                                                   */
/******************************************************************************/

//#define A                 1
//#define B                 2
//#define C                 3

/******************************************************************************/
/* Animation Function Variables                                               */
/******************************************************************************/

typedef enum
{
    Sweep,
    Fade,
    Fill,
    Travel,
    Dominator,
    AllOn,
    AllOff
            
} AnimationList;

//function pointer with Animation function signature
typedef void (*functionPointer)(struct ANIMATION_RENDER_FRAME*);

/**
 * ANIMATION_RENDER_FRAME   Struct used to store a single animation frame with variables used to control the animation
 * @param animationState    holds the state of the animation function, 0 = init, 1 = run normal, 2 = halt, 3+ = run alternate
 * @param frameCount        frameCount stores which frame the animation will render upon the next execution
 * @para  frameDelay        frameDelay is used to extend the length of a frame in the animation sequence
 * @para  frameDelayTrigger value that frameDelay will count to before it recycles
 * @param PWM0..7           PWMn is an 8 element char array that can store 255 steps but in practice is using 15. 0 if LED OFF,
 *                          anything else is LED ON 
 */
struct ANIMATION_RENDER_FRAME
{
    char animationState;
    char animationMode;
    char frameCount;
    char frameDelay;
    char frameDelayTrigger;
    char pwm[8];    
};

struct ANIMATION_OUTPUT_BUFFER
{
    char pwm[8];
};
  

/******************************************************************************/
/* Animation Function Prototypes                                              */
/******************************************************************************/
void SweepAnimation(struct ANIMATION_RENDER_FRAME* aniRender_ptr);  /*generates a larson scan animation with 8 LEDs*/

void FadeAnimation(struct ANIMATION_RENDER_FRAME* aniRender_ptr); /*generates a fade animation with a 8 LEDs*/

void FillAnimation(struct ANIMATION_RENDER_FRAME* aniRender_ptr);   /*generates a fill animation with four LEDs from 0 to 3*/

void TravelAnimation(struct ANIMATION_RENDER_FRAME* aniRender_ptr); /*generates a single lit LED that will travel from RC0 to RC4*/

void DominatorAnimation(struct ANIMATION_RENDER_FRAME* aniRender_ptr); /*generates an animation that looks like the LEDs on the Dominator weapon from PsychoPass (anime)*/

void AllOnAnimation(struct ANIMATION_RENDER_FRAME* aniRender_ptr);    /*turns all LEDs on at full brightness*/

void AllOffAnimation(struct ANIMATION_RENDER_FRAME* aniRender_ptr); /*turns all LEDs off*/
/******************************************************************************/
 /* Animation Service Functions*/
 /******************************************************************************/
functionPointer AnimationSelector(AnimationList a);

void SetFrameDelayTrigger(char delay, struct ANIMATION_RENDER_FRAME* aniRender_ptr);
