#ifndef CONNEXION_H
#define CONNEXION_H

#include "linkedlist_connexion.h"

// Public
void * gerer_connexion_client(void * descripteur_socket_client);

// Private
void fermer_ecouteur_client(connexion_client *connexion);
void afficher_erreur_non_connectee(connexion_client *connexion);
void afficher_message_bien_venue(connexion_client *connexion);
void afficher_erreur_pseudo_existant(connexion_client *connexion);
void recuperer_arguments(char *message, int spaces_a_ignorer_inferieur,int ignorer_spaces_a_partir_de, char **premier_argument, char **deuxieme_argument);

void interpreter_commande_creer(connexion_client *connexion, char *message);
void interpreter_commande_connexion(connexion_client *connexion, char *message);
void interpreter_commande_deconnexion(connexion_client * connexion);
void interpreter_message(connexion_client *connexion, char *message);
void interpreter_commande_pseudo(connexion_client *connexion, char *message);
void interpreter_commande_aide(connexion_client *connexion);
void interpreter_commande_aide_groupe(connexion_client *connexion);
void interpreter_commande_privee(connexion_client *connexion, char *message);
void interpreter_commande_groupe(connexion_client *connexion, char * message);

#endif