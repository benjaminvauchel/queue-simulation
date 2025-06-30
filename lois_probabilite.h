#ifndef LOIS_PROBABILITE_H_INCLUDED
#define LOIS_PROBABILITE_H_INCLUDED

#include <stdlib.h>
#include <math.h>

// Renvoie un nombre aléatoire généré par la loi exponentielle de paramètre lambda.
int loiExponentielle(double lambda);

// Renvoie un nombre aléatoire généré par la loi uniforme sur l'intervalle [min, max].
int loiUniformeIntervalle(int min, int max);

#endif
