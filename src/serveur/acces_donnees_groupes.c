#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

#include "codes_erreurs.h"
#include "acces_donnees_groupes.h"
#include "base_de_donnees.h"

void remplir_groupe_depuis_ligne(sqlite3_stmt *preparation_requete, groupe **groupe_cherchee)
{
    *groupe_cherchee = (groupe *)malloc(sizeof(groupe));

    (*groupe_cherchee)->id = (int)sqlite3_column_int(preparation_requete, 0);

    char *nom_groupe_base_de_donnees = (char *)sqlite3_column_text(preparation_requete, 1);
    if (nom_groupe_base_de_donnees != NULL)
    {
        (*groupe_cherchee)->nom = (char *)malloc(sizeof(char) * (strlen(nom_groupe_base_de_donnees) + 1));
        strcpy((*groupe_cherchee)->nom, nom_groupe_base_de_donnees);
    }
    else
    {
        (*groupe_cherchee)->nom = NULL;
    }
}

groupe *chercher_groupe_par_id(int id)
{
    sqlite3 *base_de_donnees = get_base_de_donnees();

    groupe *groupe_cherchee = NULL;
    char sql[] = "SELECT * FROM groupes WHERE id_groupe = ?";
    sqlite3_stmt *preparation_requete = NULL;
    int retour = sqlite3_prepare_v2(base_de_donnees, sql, -1, &preparation_requete, NULL);
    if (retour != SQLITE_OK)
    {
        printf("Erreur : %s\n", sqlite3_errmsg(base_de_donnees));
        sqlite3_close(base_de_donnees);
        exit(1);
    }

    retour = sqlite3_bind_int(preparation_requete, 1, id);

    if (retour != SQLITE_OK)
    {
        sqlite3_close(base_de_donnees);
        perror("Erreur dans la preparation de la requete sql.");
        exit(1);
    }

    while (1)
    {
        retour = sqlite3_step(preparation_requete);
        if (retour == SQLITE_ROW)
        {
            remplir_groupe_depuis_ligne(preparation_requete, &groupe_cherchee);
        }

        if (retour == SQLITE_DONE)
        {
            break;
        }
    }
    fermer_preparation_requete(preparation_requete);
    return groupe_cherchee;
}

int quitter_groupe_compte(char *pseudo)
{
    sqlite3 *base_de_donnees = get_base_de_donnees();

    char sql[] = "UPDATE comptes SET id_groupe = NULL WHERE pseudo = ?;";
    sqlite3_stmt *preparation_requete = NULL;
    int retour = sqlite3_prepare_v2(base_de_donnees, sql, -1, &preparation_requete, NULL);
    if (retour != SQLITE_OK)
    {
        printf("Erreur : %s\n", sqlite3_errmsg(base_de_donnees));
        sqlite3_close(base_de_donnees);
        exit(1);
    }

    retour = sqlite3_bind_text(preparation_requete, 1, pseudo, -1, NULL);

    if (retour != SQLITE_OK)
    {
        sqlite3_close(base_de_donnees);
        perror("Erreur dans la preparation de la requete sql.");
        exit(1);
    }

    while (1)
    {
        retour = sqlite3_step(preparation_requete);

        if (retour == SQLITE_DONE)
        {
            break;
        }
        else
        {
            printf("Erreur : %s\n", sqlite3_errmsg(base_de_donnees));
            fermer_preparation_requete(preparation_requete);
            return ERREUR;
        }
    }

    fermer_preparation_requete(preparation_requete);
    return 1;
}

int modifier_groupe_compte(char *pseudo, char *group)
{
    sqlite3 *base_de_donnees = get_base_de_donnees();
    groupe *groupe_cherchee = chercher_groupe_par_nom(group);

    char sql[] = "UPDATE comptes SET id_groupe = ? WHERE pseudo = ?;";
    sqlite3_stmt *preparation_requete = NULL;
    int retour = sqlite3_prepare_v2(base_de_donnees, sql, -1, &preparation_requete, NULL);
    if (retour != SQLITE_OK)
    {
        printf("Erreur : %s\n", sqlite3_errmsg(base_de_donnees));
        sqlite3_close(base_de_donnees);
        exit(1);
    }
    retour = sqlite3_bind_int(preparation_requete, 1, groupe_cherchee->id);
    retour = sqlite3_bind_text(preparation_requete, 2, pseudo, -1, NULL);

    if (retour != SQLITE_OK)
    {
        sqlite3_close(base_de_donnees);
        perror("Erreur dans la preparation de la requete sql.");
        exit(1);
    }

    while (1)
    {
        retour = sqlite3_step(preparation_requete);

        if (retour == SQLITE_DONE)
        {
            break;
        }
        else
        {
            printf("Erreur : %s\n", sqlite3_errmsg(base_de_donnees));
            fermer_preparation_requete(preparation_requete);
            return ERREUR;
        }
    }

    fermer_groupe(groupe_cherchee);
    fermer_preparation_requete(preparation_requete);
    return 1;
}

int ajouter_groupe(char *nom)
{
    sqlite3 *base_de_donnees = get_base_de_donnees();
    char sql[] = "INSERT INTO groupes VALUES(NULL, ?);";
    sqlite3_stmt *preparation_requete = NULL;
    int retour = sqlite3_prepare_v2(base_de_donnees, sql, -1, &preparation_requete, NULL);
    if (retour != SQLITE_OK)
    {
        printf("Erreur : %s\n", sqlite3_errmsg(base_de_donnees));
        sqlite3_close(base_de_donnees);
        exit(1);
    }

    retour = sqlite3_bind_text(preparation_requete, 1, nom, -1, 0);

    if (retour != SQLITE_OK)
    {
        sqlite3_close(base_de_donnees);
        perror("Erreur dans la preparation de la requete sql.");
        exit(1);
    }

    while (1)
    {

        retour = sqlite3_step(preparation_requete);
        if (retour == SQLITE_DONE)
        {
            break;
        }
        else
        {
            printf("Erreur : %s\n", sqlite3_errmsg(base_de_donnees));
            fermer_preparation_requete(preparation_requete);
            return ERREUR;
        }
    }

    fermer_preparation_requete(preparation_requete);
    return 1;
}

groupe *chercher_groupe_par_nom(char *nom)
{
    sqlite3 *base_de_donnees = get_base_de_donnees();
    groupe *groupe_cherchee = NULL;
    char sql[] = "SELECT * FROM groupes WHERE nom = ?";
    sqlite3_stmt *preparation_requete = NULL;
    int retour = sqlite3_prepare_v2(base_de_donnees, sql, -1, &preparation_requete, NULL);
    if (retour != SQLITE_OK)
    {
        printf("Erreur : %s\n", sqlite3_errmsg(base_de_donnees));
        sqlite3_close(base_de_donnees);
        exit(1);
    }

    retour = sqlite3_bind_text(preparation_requete, 1, nom, -1, 0);

    if (retour != SQLITE_OK)
    {
        sqlite3_close(base_de_donnees);
        perror("Erreur dans la preparation de la requete sql.");
        exit(1);
    }

    while (1)
    {
        retour = sqlite3_step(preparation_requete);
        if (retour == SQLITE_ROW)
        {
            remplir_groupe_depuis_ligne(preparation_requete, &groupe_cherchee);
        }
        else if (retour == SQLITE_DONE)
        {
            break;
        }
        else
        {
            printf("Erreur : %s\n", sqlite3_errmsg(base_de_donnees));
            fermer_preparation_requete(preparation_requete);
            return ERREUR;
        }
    }

    fermer_preparation_requete(preparation_requete);
    return groupe_cherchee;
}

void fermer_groupe(groupe *eliminer)
{
    if (eliminer != NULL)
    {
        free(eliminer->nom);
    }

    free(eliminer);
}

