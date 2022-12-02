#ifndef ECOUTEUR_SERVEUR_H
#define ECOUTEUR_SERVEUR_H

void * gerer_ecoute_serveur(void * descripteur_socket_client);
void interpreter_message(char *message);

#endif