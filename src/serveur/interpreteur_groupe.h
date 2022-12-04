#ifndef INTERPRETEUR_GROUPE_H
#define INTERPRETEUR_GROUPE_H

#include "linkedlist_connexion.h"

void interpreter_commande_groupe_message(connexion_client *connexion, char *message);
void interpreter_commande_groupe_creer(connexion_client *connexion, char *message);
void interpreter_commande_groupe_rejoindre(connexion_client *connexion, char *message);
void interpreter_commande_groupe_sortir(connexion_client *connexion);
void interpreter_commande_aide_groupe(connexion_client *connexion);

#endif