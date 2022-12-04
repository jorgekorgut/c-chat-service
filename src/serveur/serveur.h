#ifndef SERVEUR_H
#define SERVEUR_H

#include <sys/types.h>
#include <netinet/in.h>

// Public
int initialiser_serveur();
void fermer_serveur();

// Private
void initialiser_parametres_socket(int port, struct sockaddr_in *adresse_serveur, size_t tailleAdresse);
int creer_socket();
void fermer_socket(int descripteur);
void lier_socket(struct sockaddr_in *adresse_serveur, size_t tailleAdresse);
#endif