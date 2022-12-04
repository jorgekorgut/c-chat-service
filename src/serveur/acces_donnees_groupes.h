#ifndef ACCES_DONNEES_GROUPES_H
#define ACCES_DONNEES_GROUPES_H

#include <sqlite3.h>

typedef struct groupe_t
{
    int id;
    char *nom;
} groupe;

groupe *chercher_groupe_par_nom(char *nom);

groupe * chercher_groupe_par_id(int id);

int modifier_groupe_compte(char * pseudo, char * group);

int quitter_groupe_compte(char *pseudo);

int ajouter_groupe(char * nom);

void fermer_groupe(groupe *groupe);

void remplir_groupe_depuis_ligne(sqlite3_stmt *preparation_requete, groupe **groupe_cherchee);

#endif