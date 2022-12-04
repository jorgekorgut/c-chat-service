#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linkedlist_information_message.h"

int initialiser_linkedlist_information_message(linkedlist_information_message *liste)
{
    liste->debut = NULL;
    liste->taille = 0;
    return 1;
}

int ajouter_information_message(linkedlist_information_message *liste, information_message *message)
{
    message->prochain = liste->debut;
    liste->debut = message;
    liste->taille++;
    return 1;
}

int fermer_information_message(information_message *eliminer)
{
    free(eliminer->emetteur);
    free(eliminer->recepteur);
    free(eliminer->groupe_nom);
    free(eliminer->type);
    free(eliminer->message);
    free(eliminer);

    return 1;
}

int fermer_linkedlist_information_message(linkedlist_information_message *liste)
{
    information_message *iterator = liste->debut;

    while (iterator != NULL)
    {
        information_message *eliminer = iterator;
        iterator = iterator->prochain;
        fermer_information_message(eliminer);
    }
    liste->debut = NULL;
    liste->taille = 0;
    free(liste);

    return 1;
}