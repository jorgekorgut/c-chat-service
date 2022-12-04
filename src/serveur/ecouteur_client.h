#ifndef CONNEXION_H
#define CONNEXION_H

#include "linkedlist_connexion.h"

void *gerer_connexion_client(void *descripteur_socket_client);

void fermer_ecouteur_client(connexion_client *connexion);

#endif