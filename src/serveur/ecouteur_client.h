#ifndef CONNEXION_H
#define CONNEXION_H

#include "linkedlist_connexion.h"

// Public
void * gerer_connexion_client(void * descripteur_socket_client);

// Private
void interpreter_message(connexion_client *connexion, char *message);
void fermer_ecouteur_client(connexion_client *connexion);

#endif