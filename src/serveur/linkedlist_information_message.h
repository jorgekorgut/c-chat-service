#ifndef LINKEDLIST_INFORMATION_MESSAGE_H
#define LINKEDLIST_INFORMATION_MESSAGE_H

#include <time.h>

typedef struct information_message_t
{
    int id;
    char *groupe_nom;
    char *type;
    char *emetteur;
    char *recepteur;
    char *message;
    time_t date_envoye;

    struct information_message_t * prochain;
} information_message;

typedef struct linkedlist_information_message_t
{
    information_message *debut;
    int taille;
} linkedlist_information_message;

int initialiser_linkedlist_information_message(linkedlist_information_message *liste);

int ajouter_information_message(linkedlist_information_message *liste, information_message *message);

int fermer_linkedlist_information_message();

int fermer_information_message(information_message *eliminer);

#endif