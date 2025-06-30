#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <string.h>
#include <stdio.h>

typedef struct {
	int h;
	int min;
} Temps;

// Pose une question fermée (o/n) et renvoie la réponse
int questionFermee(char *question);

// Convertit un entier = temps en minutes en un Temps = temps en heures et minutes 
void intToTemps(Temps *temps, int tps);

#endif
