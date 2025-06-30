#ifndef SIMULATIONS_H_INCLUDED
#define SIMULATIONS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct {
	int nb_clients;
	float taille_moy;
	int taille_max;
	float nb_non_servis;
	int tps_rep_acc;
} Indicateurs;

// Réinitialise les indicateurs de performance à 0
void initInd(Indicateurs *ind);

// Simule la file sur une journée et enregistre dans un fichier s'il existe. 
void simulationJournee(Indicateurs *ind, FILE* fichier, float lambda, int minsrv, int maxsrv);

// Simule des files d'attente sur plusieurs jours et enregistre les informations dans un fichier (s'il existe) et selon le paramètre saveInfo.
void simulationPeriode(Indicateurs *indg, FILE* fichier, float lambda, int minsrv, int maxsrv, int nb_jours, int *saveInfo);

// Enregistre les indicateurs de performance globaux dans un fichier s'il existe (sans les noms d'indicateurs).
void enregistrerIndicateurs(FILE *fichier, int nb_jours, Indicateurs *indg, int jour_max);

// Affiche les indicateurs de performance globaux (affichage standard)
void afficherIndicateurs(int nb_jours, Indicateurs *indg);

 /* Demande à l'utilisateur les informations qu'il souhaite enregistrer (info clients, indicateurs de performance journaliers, indicateurs de performance sur la période).
 / Stocke la réponse dans un tableau de taille 3 contenant 0 (non) ou 1 (oui) avec :
 / 		saveInfo[0] : info clients
 /		saveInfo[1] : indicateurs journaliers
 /		saveInfo[2] : indicateurs globaux
 / Retourne 1 si l'entrée utilisateur est correcte, 0 sinon.
*/
int demanderInfoAEnregistrer(int *saveInfo);

// Menu interactif qui enregistre ou affiche la simulation souhaitée par l'utilisateur.
void menuSimulationPeriode();

// Enregistre les indicateurs de performances globaux de plusieurs simulations selon la valeur de lambda (variant de minlambda à maxlambda avec un pas step)
// dans un fichier prénommé selon les valeurs des paramètres.
void enregistrerIndicateursLambda(int minsrv, int maxsrv, int nb_jours, float minlambda, float maxlambda, float step);

// Même principe que la fonction que ci-dessus, mais en faisant varier maxsrv. 
void enregistrerIndicateursMaxsrv(float lambda, int minsrv, int nb_jours, int minMaxsrv, int maxMaxsrv, int step);

#endif
