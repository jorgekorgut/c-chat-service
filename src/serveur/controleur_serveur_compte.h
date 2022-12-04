#ifndef CONTROLEUR_SERVEUR_COMPTE_H
#define CONTROLEUR_SERVEUR_COMPTE_H
#include "linkedlist_connexion.h"

int changer_pseudo(connexion_client *connexion, char *nouveau_pseudo, int taille_pseudo);
int demander_changement_pseudo(connexion_client *connexion, char *pseudo, int taille_pseudo);
int creer_un_compte(connexion_client *connexion, char *pseudo, char *mot_de_passe);
int connecter_compte(connexion_client *connexion, char *pseudo, char *mot_de_passe);

#endif