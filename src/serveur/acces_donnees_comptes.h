#ifndef ACCES_DONNEES_COMPTES_H
#define ACCES_DONNEES_COMPTES_H

#include <sqlite3.h>

typedef struct compte_t
{
    int id;
    char *pseudo;
    char *mot_de_passe;
    int enligne;
    int groupe;
} compte;

void fermer_compte(compte *compte);

compte *chercher_compte_par_pseudo(char *pseudo);

compte *chercher_compte_par_id(int id);

int modifier_pseudo_compte(char *ancien, char *nouveau);

int ajouter_compte(char *pseudo, char *mot_de_passe);

void remplir_compte_depuis_ligne(sqlite3_stmt *preparation_requete, compte **compte_cherchee);

#endif