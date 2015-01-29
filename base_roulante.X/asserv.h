#ifndef ASSERV_H
#define	ASSERV_H

#define PI 3.141592
#define resolution 1024 // nombre de tic pour un tour
#define DC_MAX 100
#define Rg 57.5
#define Rd 57.5
#define V_MAX 0.05 // on fixe la vitesse max
#define Fe 0.005 // fréquence d'échantillonage


float distance(float); // calcul de la distance parcourue  en fontion du nombre de tic observÃ©

void set_position(float,float); // demande au robot de se placer une position
void routine(float, float,float,float); // routine d'asservisement
#endif	/* ASSERV */
