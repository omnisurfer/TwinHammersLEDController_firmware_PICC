/* 
 * File:   ledPWMDriver.h
 * Author: Daniel
 *
 * Created on February 9, 2014, 6:44 PM
 */

#ifndef LEDPWMDRIVER_H
#define	LEDPWMDRIVER_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* LEDPWMDRIVER_H */

/******************************************************************************/
/* ledPWMDriver #define Macros                                                */
/******************************************************************************/

#define MAX_BRIGHTNESS      64//127
#define ALPHA_VALUE         65//128
//#define C                 3

/******************************************************************************/
/* ledPWMDriver Function Variables                                                  */
/******************************************************************************/


/******************************************************************************/
/* ledPWMDriver Function Prototypes                                              */
/******************************************************************************/

char ProcessLEDPWM(struct ANIMATION_OUTPUT_BUFFER* aniOutputBuffer_ptr);