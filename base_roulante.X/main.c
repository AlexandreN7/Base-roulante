/*
 * Template dsPIC33F
 * Compiler : Microchip xC16
 * µC : 33FJ64MC802
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
#include <pps.h>

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

void __attribute__((interrupt, auto_psv)) _T2Interrupt(void) {
    state = POS1CNT;

    _T2IF = 0; // On baisse le FLAG
}

/*****************************POS1CNT********************
 * TX et RX Interrupt *
 *************************************************/
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void) {
    _U2RXIF = 0; // On baisse le FLAG
    led = !led;
}

void __attribute__((__interrupt__, no_auto_psv)) _U2TXInterrupt(void) {
    _U2TXIF = 0; // clear TX interrupt flag
}

void InitApp(void) {
    //pin de la LED en sortie
    _TRISA0 = 0;
    _TRISA1 = 0;
    //Si on a un interrupteur sur la pin RB5 (par exemple), on la met en entrée
    _TRISB5 = 1;
    //Et on active la pullup qui va bien (registres CNPU1 et CNPU2)
    _CN27PUE = 1;

    // activation de la priorité des interruptions
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
    // __builtin_write_OSCCONL(OSCCON & 0xBF);
    _U2RXR = 31;
    _RP5R = 5; // RP25 = U2TX (p.167)
    //  __builtin_write_OSCCONL(OSCCON | 0x40);
    ////////////////////////////////////////////////////////////
}

int16_t main(void) {


    TRISBbits.TRISB8=1;
    TRISBbits.TRISB7=1;
    // PMD1bits.QEI1MD = 0; //enable

    Init_All();
   // PPSInput(IN_FN_PPS_QEA1, IN_PIN_PPS_RP7);
    //PPSInput(IN_FN_PPS_QEB1, IN_PIN_PPS_RP8);
    //OpenQEI1(QEI_DIR_SEL_QEB & QEI_INT_CLK & QEI_INDEX_RESET_DISABLE & QEI_CLK_PRESCALE_1 & QEI_NORMAL_IO & QEI_MODE_x4_PULSE & QEI_UP_COUNT, 0);
    InitApp();
     _QEA1R = 7;
     _QEB1R = 6;
     DFLT1CONbits.QECK = 5;

    //    PWM_Moteurs_droit(20);
    //   PWM_Moteurs_gauche(80);

    while (1) {


            MOTOR_2A_O = 1;
        __delay_ms(800);
            MOTOR_2A_O = 0;

        //    WriteUART2(100);
        //  index = QEI1CONbits.INDX;

        __delay_ms(800);


    }
}

