#ifndef INTERPRETEUR_H
#define INTERPRETEUR_H

#include "linkedlist_connexion.h"

void interpreter_message(connexion_client *connexion, char *message);

void interpreter_commande_aide(connexion_client *connexion);

void interpreter_commande_privee(connexion_client *connexion, char *message);

void recuperer_arguments(char *message, int spaces_a_ignorer_inferieur, int ignorer_spaces_a_partir_de, char **premier_argument, char **deuxieme_argument);

void afficher_erreur(connexion_client *connexion);

void afficher_erreur_non_connectee(connexion_client *connexion);

void afficher_message_bien_venue(connexion_client *connexion);

void afficher_erreur_pseudo_existant(connexion_client *connexion);


#endif