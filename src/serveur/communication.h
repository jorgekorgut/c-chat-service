#ifndef COMMUNICATION_H
#define COMMUNICATION_H

// Public
void initialiser_ecoute_clients(int descripteur_socket, struct sockaddr_in *adresse_serveur, size_t tailleAdresse);
void envoyer_message_client(int description_socket_client, char * message, int taille_message);

// Private
void *ecouter_clients();
void fermer_communication();
#endif