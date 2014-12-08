/*
 * Template dsPIC33F
 * Compiler : Microchip xC16
 * �C : 33FJ64MC802
 * Juillet 2012
 *    ____________      _           _
 *   |___  /| ___ \    | |         | |
 *      / / | |_/ /___ | |__   ___ | |_
 *     / /  |    // _ \| '_ \ / _ \| __|
 *    / /   | |\ \ (_) | |_) | (_) | |_
 *   /_/    |_| \_\___/|____/ \___/'\__|
 *			      7robot.fr
 */

#include <p33Fxxxx.h>      /* Includes device header file                     */
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include "user.h"
//#include "header.h"        /* Function / Parameters                           */
#include <libpic30.h>
#include "motor.h"
#include "timer.h"
#include <uart.h>
#include <qei.h>

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

/* i.e. uint16_t <variable_name>; */


/******************************************************************************/
/* Configuartion                                                              */
/******************************************************************************/

// Select Oscillator and switching.
_FOSCSEL(FNOSC_FRCPLL & IESO_OFF);
// Select clock.
_FOSC(POSCMD_NONE & OSCIOFNC_ON & IOL1WAY_ON & FCKSM_CSDCMD);
// Watchdog Timer.
_FWDT(FWDTEN_OFF);
// Select debug channel.
_FICD(ICS_PGD1 & JTAGEN_OFF);


/******************************************************************************/
/* Main Program                                                               */

/******************************************************************************/
long state = 0;
long old_state = 0;

void __attribute__((interrupt, auto_psv)) _T2Interrupt(void)
{
    state = ReadQEI1();
    POS1CNT = 0;

    _T2IF = 0;           // On baisse le FLAG
}

/*****************************POS1CNT********************
* TX et RX Interrupt *
*************************************************/
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void){
    _U2RXIF = 0; // On baisse le FLAG
    led = !led;
}

void __attribute__((__interrupt__, no_auto_psv)) _U2TXInterrupt(void){
   _U2TXIF = 0; // clear TX interrupt flag
}

void InitApp(void) {
    //pin de la LED en sortie
    _TRISA0 = 0;
    _TRISA1 = 0;
    //Si on a un interrupteur sur la pin RB5 (par exemple), on la met en entr�e
    _TRISB5 = 1;
    //Et on active la pullup qui va bien (registres CNPU1 et CNPU2)
    _CN27PUE = 1;

    // activation de la priorit� des interruptions
    _NSTDIS = 0;

        OpenUART2(UART_EN & UART_IDLE_CON & UART_IrDA_DISABLE & UART_MODE_FLOW
        & UART_UEN_00 & UART_DIS_WAKE & UART_DIS_LOOPBACK
        & UART_DIS_ABAUD & UART_UXRX_IDLE_ONE & UART_BRGH_SIXTEEN
        & UART_NO_PAR_8BIT & UART_1STOPBIT,
          UART_INT_TX_BUF_EMPTY & UART_IrDA_POL_INV_ZERO
        & UART_SYNC_BREAK_DISABLED & UART_TX_ENABLE & UART_TX_BUF_NOT_FUL & UART_INT_RX_CHAR
        & UART_ADR_DETECT_DIS & UART_RX_OVERRUN_CLEAR,
          BRGVALAX12);

    ConfigIntUART2(UART_RX_INT_PR4 & UART_RX_INT_EN
                 & UART_TX_INT_PR5 & UART_TX_INT_DIS);


    // activation du timer 2
    OpenTimer2(T2_ON &
            T2_IDLE_CON &
            T2_GATE_OFF &
            T2_PS_1_64 &
            T2_SOURCE_INT, 3125); // 3125 pour 5ms
    // configuration des interruptions
    ConfigIntTimer2(T2_INT_PRIOR_4 & T2_INT_ON);

/////////////////////////////////UART///////////////////////////
    __builtin_write_OSCCONL(OSCCON & 0xBF);
    _U2RXR = 31;
    _RP5R = 5;  // RP25 = U2TX (p.167)
    __builtin_write_OSCCONL(OSCCON | 0x40);
    ////////////////////////////////////////////////////////////
}


int16_t main(void) {

    // Initialize IO ports and peripherals.
    TRISBbits.TRISB6=0;
    TRISBbits.TRISB7=0;
    Init_All();
    InitApp();
    Init_PWM();
    Init_QEI();
    PWM_Moteurs_droit(20);
    PWM_Moteurs_gauche(80);

    while (1) {

        if ((state-old_state)==0) {
            MOTOR_2A_O = 0;
        } else {
            MOTOR_2A_O = 1;

        }
        WriteUART2(100);
        state = old_state;
        

        //__delay_ms(100);


    }
}
