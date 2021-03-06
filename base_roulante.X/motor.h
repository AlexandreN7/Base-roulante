#ifndef MOTOR_H
#define	MOTOR_H

// Mode des pins de sens des moteurs
#define MOTOR_1A_TRIS TRISBbits.TRISB2
#define MOTOR_2A_TRIS TRISAbits.TRISA1
#define MOTOR_1B_TRIS TRISBbits.TRISB0
#define MOTOR_2B_TRIS TRISBbits.TRISB1

   // remapage au fil !
// Valeur des pin de sens des moteurs (output)
#define MOTOR_1A_O LATBbits.LATB2
#define MOTOR_1B_O LATAbits.LATA1

#define MOTOR_2A_O LATBbits.LATB1
#define MOTOR_2B_O LATBbits.LATB0

// min et max de P1TPER (periode du PWM en nb de cycles)
// ici min et max permettent de rester dans des fréquences comprises entre
// 26kHz et 1.2kHz pour respecter la datasheet du pont en H
#define P1TPER_MIN 1500UL
#define P1TPER_MAX 32000UL
// Duty-Cycle max (au cas ou on veut limiter la puissance moteur)
#define DC_MAX 100

// initialise les PWM
void Init_PWM(void);
// initialise le QEI (compteur pour encodeur)
void Init_QEI(void);
// Applique un PWM en ne réglant que les Duty-Cycle (compris entre -100 et 100)
// la fréquence du PWM est automatiquement réglée à 26.667 kHz (P1TPER=1500)
void PWM_Moteurs(float DC_gauche, float DC_droit);
// Applique un PWM en réglant la fréquence (1kHz-26.66kHz) et les Duty-Cycle (-100->+100)
void PWM_Moteurs_Detail(float frequence, float DC_gauche, float DC_droit);

void PWM_Moteurs_gauche(float DC);
void PWM_Moteurs_droit(float DC);

#endif	/* MOTOR_H */

