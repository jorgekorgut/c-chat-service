#ifndef CONTROLEUR_SERVEUR_H
#define CONTROLEUR_SERVEUR_H

#include "linkedlist_connexion.h"

//Public
int demander_changement_pseudo(char * pseudo, int taille_pseudo);
int ecrire_a_tous(connexion_client *emetteur, char *message);
void initialiser_controleur();
void fermer_controleur();
linkedlist_connexion_client * get_liste_connexions();

#endif