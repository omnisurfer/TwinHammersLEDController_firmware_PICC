/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "user.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

void InitApp(void)
{
    /* TODO Initialize User Ports/Peripherals/Project here */
    TRISC = 0x00, TRISA = 0x08, TRISB = 0x00;
    PORTC = 0x00, PORTA = 0x00, PORTB = 0x00;
    WPUA = 0x08;


    /* Setup analog functionality and port direction */
    ANSEL = 0x00;
    ANSELH = 0x00;

    /* Initialize OPTION_REG */
    OPTION_REGbits.T0CS = 0;
    
    /* Initialize TIMER1*/
    T1CONbits.TMR1CS = 0;
    
    //1:8 Prescale, 20MHz/4 = 200nS per tick
    T1CONbits.T1CKPS = 0x00;
    
    T1CONbits.TMR1ON = 0;
    PIE1bits.TMR1IE = 0;
    INTCONbits.PEIE = 0;

    /* Init TIMER0 */
    INTCONbits.T0IE = 1;

    //set 1:4 prescale
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS = 0x01;

    /* Enable interrupts */            
    INTCONbits.GIE = 0;
}

