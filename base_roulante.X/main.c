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
#include <libpic30.h>
#include "motor.h"
#include "timer.h"
#include "asserv.h"
#include <uart.h>
//#include "asserv.h"
/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

/* i.e. uint16_t <variable_name>; */


/******************************************************************************/
/* Configuartion               s                                               */
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
/////////////////////////////////Variables globales////////////////////////////
long old_state = 0;
int pwm1 = 0, pwm2 = 0;
int state = 0; // 0arret , 1 avant, 2 recule, 3 gauche , 4 droite
int diffg = 0,diffd =0;
unsigned int oldtics_g = 0 , oldtics_d = 0;
char data[6]={0};
////////////////////////////////////////////////////////////////////////////////




void __attribute__((interrupt, auto_psv)) _T2Interrupt(void) {

    // compteurs QEI gauche et droit
    volatile static unsigned int tics_g, tics_d;
    //         // commandes gauches et droite
    //                 // r�cup�ration des donn�es des compteurs qei gauche et droit
    tics_d = (int) POS1CNT;
    tics_g = (int) POS2CNT;

    diffg = tics_g-oldtics_g;
    diffd = tics_d-oldtics_d;

    oldtics_g=tics_g;
    oldtics_d=tics_d;

    routine(-diffg,-diffd); // routine d'asservissement

    _T2IF = 0; // On baisse le FLAG
}

/*****************************POS1CNT********************
 * TX et RX Interrupt *
 *************************************************/
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void) {
    _U2RXIF = 0; // On baisse le FLAG
    int i =0, numflag = 0;
    char input = 0;
    int cmd1=0,cmd2=0;
    input = ReadUART2();
    for (i=0;i<6;i++){
        data[i]=0;
    }
    if (input == 'X') {
        while(input != 'Y') {
          input = ReadUART2();
          data[i]=input;
          numflag =1;
          i++;
        }

    }
    }


void __attribute__((__interrupt__, no_auto_psv)) _U2TXInterrupt(void) {
    _U2TXIF = 0; // clear TX interrupt flag
}

void InitApp(void) {
    _TRISA0 = 0;
    _TRISA1 = 0;
    _TRISB5 = 1;
    _CN27PUE = 1;
    MOTOR_1A_TRIS = 0;
    MOTOR_1B_TRIS = 0;
    MOTOR_2A_TRIS = 0;
    MOTOR_2B_TRIS = 0;
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

    ConfigIntUART2(UART_RX_INT_PR5 & UART_RX_INT_EN
            & UART_TX_INT_PR5 & UART_TX_INT_DIS);


    // activation du timer 2
    OpenTimer2(T2_ON &
            T2_IDLE_CON &
            T2_GATE_OFF &
            T2_PS_1_64 &
            T2_SOURCE_INT, 3125); // 3125 pour 5ms
    // configuration des interruptions
    ConfigIntTimer2(T2_INT_PRIOR_4 & T2_INT_ON);

    /////////////////////////////////UART///////////////////////
    //RPINR14bits.
    _U2RXR = 5;
   // _RP5R = 5; // RP25 = U2TX (p.167)
    ////////////////////////////////////////////////////////////
}

int16_t main(void) {

    //char test[50]="test";
    Init_All();
    InitApp();
    DFLT1CONbits.QECK = 5;

    long i = 0;

    //PWM_Moteurs_droit(60);
    //PWM_Moteurs_gauche(-10);
    
    while (1) {
        motion_speed(2,2);
        for( i=0; i<10000;i++) {}
    }
}

