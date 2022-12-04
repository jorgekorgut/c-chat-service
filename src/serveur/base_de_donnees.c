#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "base_de_donnees.h"
#include "controleur_serveur.h"
#include "linkedlist_information_message.h"

sqlite3 *base_de_donnees;

sqlite3 * get_base_de_donnees()
{
    return base_de_donnees;
}

int initialiser_historique_messages()
{
    int retour = sqlite3_open("database/database.db", &base_de_donnees);

    if (retour != SQLITE_OK)
    {
        sqlite3_close(base_de_donnees);
        perror("Erreur dans l'ouverture de la base de donnees.");
        exit(1);
    }

    return 1;
}

void fermer_preparation_requete(sqlite3_stmt *preparation_requete)
{
    sqlite3_reset(preparation_requete);
    sqlite3_clear_bindings(preparation_requete);
    sqlite3_finalize(preparation_requete);
}

int fermer_historique_messages()
{
    sqlite3_close(base_de_donnees);
    return 1;
}