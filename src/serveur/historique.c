#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "historique.h"

sqlite3 *base_de_donnees;

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

int ajouter_message(information_message *message)
{
    char sql[] = "INSERT INTO historique_messages VALUES(NULL, ?,?,?,?);";
    sqlite3_stmt *preparation_declaration = NULL;
    int retour = sqlite3_prepare_v2(base_de_donnees, sql, -1, &preparation_declaration, NULL);
    if (retour != SQLITE_OK)
    {
        printf("Erreur : %s\n", sqlite3_errmsg(base_de_donnees));
        sqlite3_close(base_de_donnees);
        exit(1);
    }

    retour |= sqlite3_bind_text(preparation_declaration, 1, message->groupe, -1, 0);

    retour |= sqlite3_bind_text(preparation_declaration, 2, message->emetteur, -1, 0);

    retour |= sqlite3_bind_text(preparation_declaration, 3, message->recepteur, -1, 0);

    retour |= sqlite3_bind_text(preparation_declaration, 4, message->message, -1, 0);

    if (retour != SQLITE_OK)
    {
        sqlite3_close(base_de_donnees);
        perror("Erreur dans la preparation de la declaration sql.");
        exit(1);
    }

    while (1)
    {
        retour = sqlite3_step(preparation_declaration);
        if(retour == SQLITE_ROW)
        {

        }
        
        if (retour == SQLITE_DONE)
        {
            break;
        }
    }
    sqlite3_reset(preparation_declaration);
    sqlite3_clear_bindings(preparation_declaration);
    sqlite3_finalize(preparation_declaration);
    return 1;
}

int fermer_historique_messages()
{
    sqlite3_close(base_de_donnees);
    return 0;
}