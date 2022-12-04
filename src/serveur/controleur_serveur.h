#ifndef CONTROLEUR_SERVEUR_H
#define CONTROLEUR_SERVEUR_H

#define PSEUDO_SERVEUR "serveur"

#include "linkedlist_connexion.h"

void initialiser_controleur();
void fermer_controleur();
linkedlist_connexion_client *get_liste_connexions();

#endif