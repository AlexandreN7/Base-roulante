#ifndef ASSERV_H
#define	ASSERV_H

#define PI 3.141592
#define resolution 1024 // nombre de tic pour un tour
#define DC_MAX 100
#define Rg 0.02888
#define Rd 0.0257
#define V_MAX 0.5 // on fixe la vitesse max
#define Fe 200 // fréquence d'échantillonage

extern float cmdd ;
extern float cmdg ;


float distance(float); // calcul de la distance parcourue  en fontion du nombre de tic observÃ©

void set_position(float,float); // demande au robot de se placer une position
void routine(int,int); // routine d'asservisement
void motion_speed(float,float);
#endif	/* ASSERV */
