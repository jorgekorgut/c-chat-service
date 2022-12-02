#ifndef CONNEXION_H
#define CONNEXION_H

#include "linkedlist_connexion.h"

// Public
void * gerer_connexion_client(void * descripteur_socket_client);

// Private
void fermer_ecouteur_client(connexion_client *connexion);
void afficher_erreur_pseudo_non_renseigne(connexion_client *connexion);
void interpreter_message(connexion_client *connexion, char *message);
void interpreter_commande_pseudo(connexion_client *connexion, char *message);
void interpreter_commande_aide(connexion_client *connexion);
void interpreter_commande_aide_groupe_aide(connexion_client *connexion);
void interpreter_commande_prive(connexion_client *connexion, char *message);

#endif