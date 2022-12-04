#ifndef CONTROLEUR_SERVEUR_GROUPE_H
#define CONTROLEUR_SERVEUR_GROUPE_H
#include "linkedlist_connexion.h"

int rejoindre_groupe(connexion_client *connexion, char *groupe_nom);
int quitter_groupe(connexion_client *connexion);
int creer_groupe(connexion_client *connexion, char *groupe_nom);

#endif