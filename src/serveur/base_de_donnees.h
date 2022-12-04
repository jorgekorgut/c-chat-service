#ifndef HISTORIQUE_H
#define HISTORIQUE_H

#include <sqlite3.h>
#include "linkedlist_information_message.h"

// Public

typedef struct compte_t
{
    int id;
    char *pseudo;
    char *mot_de_passe;
    int enligne;
    int groupe;
} compte;

typedef struct groupe_t
{
    int id;
    char *type_groupe;
    char *nom;
} groupe;

int initialiser_historique_messages();

int fermer_historique_messages();

void fermer_compte(compte *compte);

void fermer_groupe(groupe *groupe);

linkedlist_information_message *chercher_historique_messages_pseudo(char *pseudo);

void remplir_compte_depuis_ligne(sqlite3_stmt *preparation_requete, compte **compte_cherchee);

compte *chercher_compte_par_pseudo(char *pseudo);

compte *chercher_compte_par_id(int id);

groupe *chercher_groupe_par_nom(char *nom);

int ajouter_message(information_message *message);

int modifier_pseudo_compte(char *ancien, char *nouveau);

int ajouter_compte(char *pseudo, char *mot_de_passe);

// Privee
void fermer_preparation_requete(sqlite3_stmt *preparation_requete);

#endif