#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct _Client {
	int h_arrivee;
	int h_debut_srv;
	int h_fin_srv;
	struct _Client *prec; 
} Client;

typedef struct {
	int taille;
	Client *queue;
} File;				// Ne pas confondre File et FILE

// Initialise une file vide.
void initFile(File *file);

// Ajoute un client à la queue de la file.
void enfiler(File *file, Client *client);

// Supprime le client en tête de file et modifie **premier de sorte qu'il pointe sur le nouveau premier client.
void defiler(File *file, Client **premier);

// Modifie les informations relatives à un client passé en paramètre
void creerClient(Client *client, int h_arrivee, int h_debut_srv, int h_fin_srv);

// Enregistre le client spécifié dans le fichier avec les informations relatives à son service.
void enregistrerClient(FILE* fichier, Client *client);

#endif
