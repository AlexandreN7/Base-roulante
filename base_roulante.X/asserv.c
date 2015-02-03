#include "asserv.h"
#include "motor.h"
#include <math.h>

//////////////////////////////////COEFFICIENTS////////////////////////////////
float kp_d = 100;
float kp_g = 100;

float ki_d = 100;
float ki_g = 100;

float kd_d = 0;
float kd_g = 0;
//////////////////////////////////PHYSIQUE////////////////////////////////////
float p_x=0;
float p_y=0;
float angle=0;
float v_x=0;
float v_y=0;


float cmdd =0; // COMMANDE D/G
float cmdg =0;

float int_erreur_d = 0; // SOMME DES ERREURS
float int_erreur_g = 0;

float detla_erreur_d = 0; // DIFF2RENCE DES ERREURS
float detla_erreur_g  = 0;


float distance(float consigne) // renvoit la distance parcourue 
{
    float result = 0;
    result = 2*PI*consigne/resolution;
    return result;
}



void set_position(float x,float y)
{// demande au robot de se placer une position

}

void routine (int diffg, int diffd)
{
    float vitesseMesure_g = 2*PI*Rg/resolution*diffg*Fe;
    float vitesseMesure_d = 2*PI*Rd/resolution*diffd*Fe;

    float erreur_g = cmdg-vitesseMesure_g;
    float erreur_d = cmdd-vitesseMesure_d;

    
    int_erreur_g=int_erreur_g+erreur_g; // on somme les erreurs pour le coefficient intégrateur
    int_erreur_d=int_erreur_d+erreur_d;

    detla_erreur_g =erreur_g-detla_erreur_g; // différences les erreurs pour le dérivateur
    detla_erreur_d =erreur_d-detla_erreur_d ;

    float correction_d = kp_d*erreur_d +ki_d*int_erreur_d-kd_d*detla_erreur_d;
    float correction_g = kp_g*erreur_g +ki_g*int_erreur_g-kd_g*detla_erreur_g;

    //cmdd = cmdd + correction_d*100/V_MAX;
    //cmdg = cmdg + correction_g*100/V_MAX;
    if (correction_d> 100)
    {
        PWM_Moteurs_droit(100);
        PWM_Moteurs_gauche(100);
    }
    else if (correction_d< -100)
    {
        PWM_Moteurs_droit(-100);
        PWM_Moteurs_gauche(-100);

    }
    else {
        PWM_Moteurs_droit(-correction_d);
        PWM_Moteurs_gauche(-correction_g);
   }
}


void motion_speed(float vitessed , float vitesseg) // envoit d'une nouvelle vitesse
{
    cmdd = vitessed;
    cmdg = vitesseg;

    detla_erreur_g=0;
    detla_erreur_d=0;

    int_erreur_d=0;
    int_erreur_g=0;
}