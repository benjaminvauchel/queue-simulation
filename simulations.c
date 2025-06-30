#include "simulations.h"
#include "lois_probabilite.h"
#include "utils.h"
#include "file.h"
#include <math.h>

#define TAILLE_MAX 100
#define MAX(a,b) (((a)>(b)) ? (a) : (b))

void initInd(Indicateurs *ind) {
	ind->nb_clients = 0;
	ind->taille_moy = 0.;
	ind->taille_max = 0;
	ind->nb_non_servis = 0.;
	ind->tps_rep_acc = 0;
}

void simulationJournee(Indicateurs *ind, FILE* fichier, float lambda, int minsrv, int maxsrv) {
	
	// Initialisation des indicateurs de performance
	initInd(ind);
	ind->nb_clients++;
	
	if (fichier != NULL) {fprintf(fichier, "Heure d'arrivée,Durée d'attente,Heure début de service,Heure fin de service\n");}
	
	int t = 510; // Initialisation du temps à 7h30min = 510 min (début de la journée)
	
	File file; // Initialisation de la file.
	initFile(&file);
	
	int ferme = 0; // Vaudra 1 lorsque le dernier client arrivera. Il ne pourra alors plus y avoir de nouveau client (ligne 109).
	
	// Définition du premier client de la journée.
	t = t + loiExponentielle(lambda);
	int duree_srv = loiUniformeIntervalle(minsrv, maxsrv);
	Client *premier = (Client *) malloc(sizeof(Client));
	creerClient(premier, t, t, t+duree_srv);
	enfiler(&file, premier);
	enregistrerClient(fichier, premier);
	
	while (t <= 1050) {
	
		// Si c'est la fin du service du client en tête, on défile.
		if ((premier != NULL) && (t == premier->h_fin_srv)) { // QUAND-MÊME BIZARRE QUE (premier == NULL) SE REALISE !!!
			defiler(&file, &premier);
		}

		if ((ferme == 0) && (t == file.queue->h_arrivee)) {
			int h_arrivee = t + loiExponentielle(lambda);
			
			// Compense l'incrémentation de t et de taille_moy plus loin. Permet de traiter plusieurs clients s'ils arrivent la même heure (sans compter la taille de la file en double).
			if (t == h_arrivee) {
				t--;
				ind->taille_moy = ind->taille_moy - file.taille;
			}
			
			// Si le dernier client arrive avant 17h on l'insert, sinon il n'est pas pris en compte et on ferme (ferme = 1).
			if (h_arrivee <= 1020) {
				int h_debut_srv = MAX(h_arrivee, file.queue->h_fin_srv);
				int duree_srv = loiUniformeIntervalle(minsrv, maxsrv);
				int h_fin_srv = h_debut_srv + duree_srv;
				// Si le dernier client servi ne peut pas être totalement servi, on le compte dans les non servis et on ne prend pas en compte sa durée de service.
				if (h_fin_srv > 17*60+30) {
					ind->nb_non_servis++;
				}
				else {
					ind->tps_rep_acc = ind->tps_rep_acc + h_fin_srv - h_arrivee;
				}
				Client *nouveau = (Client *) malloc(sizeof(Client));
				creerClient(nouveau, h_arrivee, h_debut_srv, h_fin_srv);
				enfiler(&file, nouveau);
				enregistrerClient(fichier, nouveau);
				ind->nb_clients++;
			}
			else {
				ferme = 1;
			}
		}
		t++;
		ind->taille_moy = ind->taille_moy + file.taille;
		ind->taille_max = MAX(ind->taille_max, file.taille);
	}
	ind->taille_moy = ind->taille_moy / (9 * 60);
}

void simulationPeriode(Indicateurs *indg, FILE* fichier, float lambda, int minsrv, int maxsrv, int nb_jours, int *saveInfo) {
	
	int jour_max = 1;
	Indicateurs *ind = (Indicateurs *) malloc(sizeof(Indicateurs));
	initInd(indg);
	
	// Parcours de chaque jour de la période
	for (int i = 1; i <= nb_jours; i++) {
		
		// Enregistrement du jour i avec ses indicateurs dans le fichier à condition que l'utilisateur l'ait souhaité
		if ((fichier != NULL) && (saveInfo[0] || saveInfo[1])) {fprintf(fichier, "\nJour %d\n", i);}
		// Permet d'enregistrer ou non les info clients de chaque jour (selon le souhait de l'utilisateur) en une ligne
		saveInfo[0] ? simulationJournee(ind, fichier, lambda, minsrv, maxsrv) : simulationJournee(ind, NULL, lambda, minsrv, maxsrv);
		// Calcul des indicateurs de performance de la journée
		float debit_journ = (float) ind->nb_clients / 540.;
		float ind_taux_non_servis = 100 * (float) ind->nb_non_servis / (float) ind->nb_clients;
		int ind_tps_rep_moy = (int) round ((float) ind->tps_rep_acc / (ind->nb_clients - ind->nb_non_servis));
		// Enregistrement des indicateurs de performance de la journée si souhaité par l'utilisateur
		if ((fichier != NULL) && saveInfo[1]) {
			Temps ind_temps_rep_moy;
			intToTemps(&ind_temps_rep_moy, ind_tps_rep_moy);
			fprintf(fichier, "Taille moyenne,Taille max,Débit journalier,Taux de non servis,Temps de réponse moyen\n");
			fprintf(fichier, "%f,%d,%f,%f %%,%02d:%02d\n", ind->taille_moy, ind->taille_max, debit_journ, ind_taux_non_servis, ind_temps_rep_moy.h, ind_temps_rep_moy.min);
		}
		// Ajout des indicateurs du jour dans les indicateurs sur la période
		indg->nb_clients = indg->nb_clients + ind->nb_clients;
		indg->nb_non_servis = indg->nb_non_servis + ind->nb_non_servis;
		indg->taille_moy = indg->taille_moy + ind->taille_moy;
		if (ind->taille_max > indg->taille_max) {
			indg->taille_max = ind->taille_max;
			jour_max = i;
		}
		indg->tps_rep_acc = indg->tps_rep_acc + ind->tps_rep_acc;
		
	}
	
	// Enregistrement des indicateurs sur la période si souhaité par l'utilisateur
	if ((fichier != NULL) && saveInfo[2]) {
		fprintf(fichier, "\nToute la période\n");
		fprintf(fichier, "Taille moyenne,Taille max (jour),Débit journalier moyen,Taux de non servis,Temps de réponse moyen\n");
		enregistrerIndicateurs(fichier, nb_jours, indg, jour_max);
	}

}

void enregistrerIndicateurs(FILE *fichier, int nb_jours, Indicateurs *indg, int jour_max) {
	if (fichier != NULL) {
		float debit_journ_moy = (float) indg->nb_clients / (nb_jours * 540.);
		float taux_non_servis = 100 * (float) indg->nb_non_servis / (float) indg->nb_clients;
		int tps_rep_moy = (int) round ((float) indg->tps_rep_acc / (indg->nb_clients - indg->nb_non_servis));
		Temps temps_rep_moy;
		intToTemps(&temps_rep_moy, tps_rep_moy);
		if (jour_max == 0) {
			fprintf(fichier, "%f,%d,%f,%f %%,%02d:%02d\n", indg->taille_moy / nb_jours, indg->taille_max, debit_journ_moy, taux_non_servis, temps_rep_moy.h, temps_rep_moy.min);
		}
		else {
			fprintf(fichier, "%f,%d (jour %d),%f,%f %%,%02d:%02d\n", indg->taille_moy / nb_jours, indg->taille_max, jour_max, debit_journ_moy, taux_non_servis, temps_rep_moy.h, temps_rep_moy.min);
		}
	}
}

void afficherIndicateurs(int nb_jours, Indicateurs *indg) {
	float debit_journ_moy = (float) indg->nb_clients / (nb_jours * 540.);
	float taux_non_servis = 100 * (float) indg->nb_non_servis / (float) indg->nb_clients;
	int tps_rep_moy = (int) round ((float) indg->tps_rep_acc / (indg->nb_clients - indg->nb_non_servis));
	Temps temps_rep_moy;
	intToTemps(&temps_rep_moy, tps_rep_moy);	
	printf("Taille moyenne : %f\nTaille max : %d\nDébit journalier moyen : %f\nTaux de non servis : %f %%\nTemps de réponse moyen : %02d:%02d\n",
			indg->taille_moy / nb_jours, indg->taille_max, debit_journ_moy, taux_non_servis, temps_rep_moy.h, temps_rep_moy.min);
}

int demanderInfoAEnregistrer(int *saveInfo){
	saveInfo[0] = 0; saveInfo[1] = 0; saveInfo[2] = 0;
	printf("Veuillez sélectionner les informations que vous souhaitez enregistrer.\n");
	printf("\t1. Informations relatives à chaque client\n");
	printf("\t2. Indicateurs de performance par jour\n");
	printf("\t3. Indicateurs de performance sur la période\n");
	printf("Exemples : 23 ; 321 ; 1\n");
	int ans;
	scanf("%d", &ans);
	int chiffre = ans % 10;
	while (chiffre != 0) {
		switch(chiffre) {
			case 1:
				saveInfo[0] = 1;
				break;;
			case 2:
				saveInfo[1] = 1;
				break;
			case 3:
				saveInfo[2] = 1;
				break;
			default:
				return 0;
		}
		ans = ans / 10;
		chiffre = ans % 10;
	}
	return 1;
}

void menuSimulationPeriode(){
	float lambda = 0.;
	int minsrv = 0, maxsrv = 0, nb_jours = 0;
	char nom_fic[TAILLE_MAX];
	while (lambda <= 0.) {
		printf("lambda ? ");
		scanf("%f", &lambda);
	}
	while (minsrv <= 0) {
		printf("Temps de service min ? ");
		scanf("%d", &minsrv);
	}
	while (maxsrv <= 0) {
		printf("Temps de service max ? ");
		scanf("%d", &maxsrv);
	}
	while (nb_jours <= 0) {
		printf("Durée de la période (en jours) ? ");
		scanf("%d", &nb_jours);
	}

	FILE *fichier;
	int saveInfo[3] = {0, 0, 0};
	// Enregistrement de tous les indicateurs dans un fichier spécifié
	if (questionFermee("Enregistrer ?")) {
		printf("Fichier d'enregistrement ? ");
		scanf("%s", nom_fic);
		fichier = fopen(nom_fic, "w");
		int repCorrecte = 0;
		while (!repCorrecte) {
			repCorrecte = demanderInfoAEnregistrer(saveInfo);
		}
	}
	
	Indicateurs *indg = (Indicateurs *) malloc(sizeof(Indicateurs));
	simulationPeriode(indg, fichier, lambda, minsrv, maxsrv, nb_jours, saveInfo);	
	
	// Affichage des indicateurs globaux
	if (questionFermee("Afficher les indicateurs de performance globaux ?")) {
		afficherIndicateurs(nb_jours, indg);
	}
	
	if (fichier != NULL) {fclose(fichier);}
}

void enregistrerIndicateursLambda(int minsrv, int maxsrv, int nb_jours, float minlambda, float maxlambda, float step) {

	char nom_fic[TAILLE_MAX];
	sprintf(nom_fic, "indicateurs_minsrv=%d_maxsrv=%d_nbjours=%d_lambda{%.4f,%.4f,%.4f}.csv", minsrv, maxsrv, nb_jours, minlambda, maxlambda, step);
	FILE *fichier = fopen(nom_fic, "w");

	fprintf(fichier, "lambda,Taille moyenne,Taille max,Débit journalier moyen,Taux de non servis,Temps de réponse moyen\n");	
	
	Indicateurs *indg = (Indicateurs *) malloc(sizeof(Indicateurs));
	for (float lambda = minlambda; lambda <= maxlambda; lambda = lambda + step) {
//		printf("%f\n", lambda);
		fprintf(fichier, "%.4f,", lambda);
		int saveInfo[3] = {0, 0, 0};
		simulationPeriode(indg, NULL, lambda, minsrv, maxsrv, nb_jours, saveInfo);
		enregistrerIndicateurs(fichier, nb_jours, indg, 0);
	}

	fclose(fichier);
}

void enregistrerIndicateursMaxsrv(float lambda, int minsrv, int nb_jours, int minMaxsrv, int maxMaxsrv, int step) {
	
	char nom_fic[TAILLE_MAX];
	sprintf(nom_fic, "indicateurs_lambda=%.4f,minsrv=%d_maxsrv={%d,%d,%d}_nbjours=%d.csv", lambda, minsrv, minMaxsrv, maxMaxsrv, step, nb_jours);
	FILE *fichier = fopen(nom_fic, "w");

	fprintf(fichier, "maxsrv,Taille moyenne,Taille max,Débit journalier moyen,Taux de non servis,Temps de réponse moyen\n");	
	
	Indicateurs *indg = (Indicateurs *) malloc(sizeof(Indicateurs));
	for (int maxsrv = minMaxsrv; maxsrv <= maxMaxsrv; maxsrv = maxsrv + step) {
//		printf("%d\n", maxsrv);
		fprintf(fichier, "%d,", maxsrv);
		int saveInfo[3] = {0, 0, 0};
		simulationPeriode(indg, NULL, lambda, minsrv, maxsrv, nb_jours, saveInfo);
		enregistrerIndicateurs(fichier, nb_jours, indg, 0);
	}

	fclose(fichier);

}

