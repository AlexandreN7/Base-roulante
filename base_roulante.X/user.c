#include <xc.h>



#include <stdint.h>          /* For uint16_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include <timer.h>

#include "user.h"          /* User funct/params, such as InitApp              */
#include "qei.h"           /* QEI definitions for easier use                  */
#include <libpic30.h>
//#include "tests.h"
#include "lib_asserv/lib_asserv.h"
//#include "atp-asserv.h"
//#include "atp.h"
//#include "sick.h"
#include "motor.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

void ConfigureOscillator(void)
{
    // Configure PLL prescaler, PLL postscaler, PLL divisor
    PLLFBDbits.PLLDIV = 150;        // M=152
    CLKDIVbits.PLLPRE  = 5;         // N1=7
    CLKDIVbits.PLLPOST = 0;         // N2=2
    /* Fosc = M/(N1.N2)*Fin
     * Fin : 7.37MHz (quartz interne)
     * Fosc � 80 MHZ (ne doit pas d�passer 80 MHZ)
     * la solution la plus proche est 152*7.37/(7*2) = 80.017
     * attention, cela entraine donc une FCY et une FP�riph�riques � 40 MHZ
     */
    while (!OSCCONbits.LOCK);       // attente que la PLL soit lock�e sur se nouvelle configuration.
}

// initialize all things
void Init_All(){
    /* Configure the oscillator for the device */
    ConfigureOscillator();
    /* Initialize IO ports and peripherals */
   // InitTimers();
    __delay_ms(500);
    Init_PWM();
    Init_QEI();
 //   InitSick();
   // motion_init(SendDone);
   // AtpInit();
   // __delay_ms(500);
   // Init_CN();
   // SendBoardId();
   // init_arm(1);
   // init_arm(2);
}
