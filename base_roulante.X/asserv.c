
#include "asserv.h"

//////////////////////////////////COEFFICIENTS////////////////////////////////
float kd=0;
float ki=0;
float kp=0;

//////////////////////////////////PHYSIQUE////////////////////////////////////
float p_x=0;
float p_y=0;
float angle=0;
float v_x=0;
float v_y=0;

/////////////////////////////////POSITION_REF////////////////////////////////
float p_xref =0; // on place le robot à la position (0,0)
float p_yref =0;

float v_xref =0; // la vitesse d'origine est fixé à 0
float v_yref =0;

float angle_ref =0; // l'angle de base est à 0;



float distance(float consigne) // renvoit la distance parcourue 
{
    float result = 0;
    result = 2*PI*consigne/resolution;
    return result;
}



void set_position(float x,float y)
{// demande au robot de se placer une position





}
