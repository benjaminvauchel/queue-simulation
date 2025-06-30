#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "lois_probabilite.h"
#include "file.h"
#include "simulations.h"

#define TAILLE_MAX 100

int main() {
	srand(time(NULL));

/*
	//Simulation d'une journée
 	Indicateurs ind;
 	FILE *fichier = fopen("test.csv", "w");
 	simulationJournee(&ind, fichier, 0.04, 5, 30);
 	fclose(fichier);
*/

/*		//Simulation sur une période
 	Indicateurs indg;
 	FILE *fichier = fopen("test.csv", "w");
 	int saveInfo[3] = {0, 0, 1};
 	simulationPeriode(&indg, fichier, 0.04, 5, 30, 30, saveInfo);
 	fclose(fichier);
*/

	menuSimulationPeriode();

//	enregistrerIndicateursLambda(5, 50, 1000, 0.001, 0.1, 0.001);
//	enregistrerIndicateursLambda(1, 30, 1000, 0.04, 2, 0.01);

//	enregistrerIndicateursMaxsrv(0.04, 1, 1000, 2, 90, 1);
	
	return 0;
}

