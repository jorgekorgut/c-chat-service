#ifndef INTERPRETEUR_COMPTE_H
#define INTERPRETEUR_COMPTE_H

#include "linkedlist_connexion.h"

void interpreter_commande_creer(connexion_client *connexion, char *message);

void interpreter_commande_connexion(connexion_client *connexion, char *message);

void interpreter_commande_deconnexion(connexion_client *connexion);

void interpreter_commande_pseudo(connexion_client *connexion, char *message);

#endif