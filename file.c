#include "file.h"
#include "utils.h"

void initFile(File *file) {
	file->queue = NULL;
	file->taille = 0;
}

void enfiler(File *file, Client *client) {
	client->prec = file->queue;
	file->queue = client;
	file->taille++;
}

void defiler(File *file, Client **premier) {
	Client *courant = file->queue;
	
	if (courant != NULL) {
		Client *suivant = (Client *) malloc(sizeof(Client));
		suivant->prec = courant;
		
		if (courant->prec == NULL) {
			file->queue = NULL;
			*premier = NULL;
		}
		
		else {
			while (courant->prec != NULL) {
				suivant = courant;
				courant = courant->prec;
			}
			suivant->prec = NULL;
			*premier = suivant;
		}
		
		file->taille--;
	}
	
}

void creerClient(Client *client, int h_arrivee, int h_debut_srv, int h_fin_srv) {
	client->h_arrivee = h_arrivee;
	client->h_debut_srv = h_debut_srv;
	client->h_fin_srv = h_fin_srv;
}

void enregistrerClient(FILE* fichier, Client *client) {

	if (fichier != NULL) {
		Temps h_arrivee;
		intToTemps(&h_arrivee, client->h_arrivee);
		Temps duree_attente;
	
		// Cas où le client ne peut pas être servi.
		if (client->h_debut_srv >= 17 * 60 + 30) {
			intToTemps(&duree_attente, 17*60+30 - client->h_arrivee);
			fprintf(fichier, "%02d:%02d,%02d:%02d\n", h_arrivee.h, h_arrivee.min, duree_attente.h, duree_attente.min);
		}

		else {
			intToTemps(&duree_attente, client->h_debut_srv - client->h_arrivee);
			Temps h_debut_srv;
			intToTemps(&h_debut_srv, client->h_debut_srv);
			Temps h_fin_srv;
			
			// Cas où le client ne peut pas être totalement servi.		
			if (client->h_fin_srv > 17 * 60 + 30) {
				intToTemps(&h_fin_srv, 17*60 + 30);
				fprintf(fichier, "%02d:%02d,%02d:%02d,%02d:%02d,%02d:%02d (!)\n", h_arrivee.h, h_arrivee.min, duree_attente.h, duree_attente.min, h_debut_srv.h, h_debut_srv.min, h_fin_srv.h, h_fin_srv.min);
			}
			else {
				intToTemps(&h_fin_srv, client->h_fin_srv);
				fprintf(fichier, "%02d:%02d,%02d:%02d,%02d:%02d,%02d:%02d\n", h_arrivee.h, h_arrivee.min, duree_attente.h, duree_attente.min, h_debut_srv.h, h_debut_srv.min, h_fin_srv.h, h_fin_srv.min);
			}
		}
	}
}
