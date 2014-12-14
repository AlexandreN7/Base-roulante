#ifndef ASSERV_H
#define	ASSERV_H

#define PI 3.141592
#define resolution 1024 // nombre de tic pour un tour


float distance(float); // calcul de la distance parcourue  en fontion du nombre de tic observé

void set_position(float,float); // demande au robot de se placer une position
float routine(float, float); // routine d'asservisement
#endif	/* ASSERV */
