#ifndef CONTROLEUR_SERVEUR_MESSAGE_H
#define CONTROLEUR_SERVEUR_MESSAGE_H
#include "linkedlist_connexion.h"

int envoyer_historique_au_compte(connexion_client *connexion);
int envoyer_message_a_tous(connexion_client *connexion, char *message);
int envoyer_message_privee(connexion_client *connexion, char *message, char *pseudo_recepteur);
int envoyer_message_groupe(connexion_client *connexion, char *groupe_nom, char *message);
void formater_message(char *pseudo, char *type_message, char *groupe, char *destinataire, char *message, char *message_formatee, time_t date);

#endif