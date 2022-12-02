#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

//Public
void fermer_communication();
void initialiser_communication();
void ecrire_message_au_serveur(char * message, int taille_message);

//Private
void initialiser_parametres(int port, char* addresse, struct sockaddr_in* adresseServeur, size_t tailleAdresse);
void creer_socket_serveur();
void connecter_avec_serveur( struct sockaddr_in* adresseServeur, size_t tailleAdresse);
void creer_processus_ecoute();
void fermer_socket();
#endif