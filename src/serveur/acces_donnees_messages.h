#ifndef ACCES_DONNEES_MESSAGES_H
#define ACCES_DONNEES_MESSAGES_H

#include "linkedlist_information_message.h"

linkedlist_information_message *chercher_historique_messages_pseudo(char *pseudo);

int ajouter_message(information_message *message);

#endif