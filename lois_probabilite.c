#include "lois_probabilite.h"
#include <stdlib.h>
#include <math.h>

// Retourne un nombre généré par la loi exponentielle de paramètre lambda
int loiExponentielle(double lambda) {
	double U = (double)rand() / (double)((unsigned)RAND_MAX + 1);
	return floor(-log(1 - U) / lambda);
}

// Retourne un nombre généré par la loi uniforme sur l'intervalle [min, max]
int loiUniformeIntervalle(int min, int max) {
	return min + (int) (rand() / (double) RAND_MAX * (max - min + 1));
}
