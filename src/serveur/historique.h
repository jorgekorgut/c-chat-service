#ifndef HISTORIQUE_H
#define HISTORIQUE_H

//Public

typedef struct information_message_t {
    int id;
    char * groupe;
    char * emetteur;
    char * recepteur;
    char * message;

    struct information_message_t * prochain;
    struct information_message_t * ancient;
} information_message;

int initialiser_historique_messages();

int ajouter_message(information_message * message);

int fermer_historique_messages();

#endif