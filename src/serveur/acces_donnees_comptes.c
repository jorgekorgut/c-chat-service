#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "base_de_donnees.h"
#include "acces_donnees_comptes.h"
#include "codes_erreurs.h"

void remplir_compte_depuis_ligne(sqlite3_stmt *preparation_requete, compte **compte_cherchee)
{
    (*compte_cherchee) = (compte *)malloc(sizeof(compte));

    (*compte_cherchee)->id = (int)sqlite3_column_int(preparation_requete, 0);

    char *pseudo_base_de_donees = (char *)sqlite3_column_text(preparation_requete, 1);
    (*compte_cherchee)->pseudo = (char *)malloc(sizeof(char) * (strlen(pseudo_base_de_donees) + 1));
    strcpy((*compte_cherchee)->pseudo, pseudo_base_de_donees);

    char *mot_de_passe_base_de_donees = (char *)sqlite3_column_text(preparation_requete, 2);
    (*compte_cherchee)->mot_de_passe = (char *)malloc(sizeof(char) * (strlen(mot_de_passe_base_de_donees) + 1));
    strcpy((*compte_cherchee)->mot_de_passe, mot_de_passe_base_de_donees);

    (*compte_cherchee)->enligne = (int)sqlite3_column_int(preparation_requete, 3);

    (*compte_cherchee)->groupe = (int)sqlite3_column_int(preparation_requete, 4);
}

void fermer_compte(compte *eliminer)
{
    if (eliminer != NULL)
    {
        free(eliminer->mot_de_passe);
        free(eliminer->pseudo);
    }

    free(eliminer);
}

compte *chercher_compte_par_pseudo(char *pseudo)
{
    sqlite3 *base_de_donnees = get_base_de_donnees();
    compte *compte_cherchee = NULL;
    char sql[] = "SELECT * FROM comptes WHERE pseudo = ?";
    sqlite3_stmt *preparation_requete = NULL;
    int retour = sqlite3_prepare_v2(base_de_donnees, sql, -1, &preparation_requete, NULL);
    if (retour != SQLITE_OK)
    {
        printf("Erreur : %s\n", sqlite3_errmsg(base_de_donnees));
        sqlite3_close(base_de_donnees);
        exit(1);
    }

    retour = sqlite3_bind_text(preparation_requete, 1, pseudo, -1, 0);

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
            remplir_compte_depuis_ligne(preparation_requete, &compte_cherchee);
        }

        if (retour == SQLITE_DONE)
        {
            break;
        }
    }
    fermer_preparation_requete(preparation_requete);
    return compte_cherchee;
}

compte *chercher_compte_par_id(int id)
{
    sqlite3 *base_de_donnees = get_base_de_donnees();
    compte *compte_cherchee = NULL;
    char sql[] = "SELECT * FROM comptes WHERE id_compte = ?";
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
            remplir_compte_depuis_ligne(preparation_requete, &compte_cherchee);
        }

        if (retour == SQLITE_DONE)
        {
            break;
        }
    }
    fermer_preparation_requete(preparation_requete);
    return compte_cherchee;
}

int ajouter_compte(char *pseudo, char *mot_de_passe)
{
    sqlite3 *base_de_donnees = get_base_de_donnees();
    char sql[] = "INSERT INTO comptes VALUES(NULL, ?, ?, 0, NULL);";
    sqlite3_stmt *preparation_requete = NULL;
    int retour = sqlite3_prepare_v2(base_de_donnees, sql, -1, &preparation_requete, NULL);
    if (retour != SQLITE_OK)
    {
        printf("Erreur : %s\n", sqlite3_errmsg(base_de_donnees));
        sqlite3_close(base_de_donnees);
        exit(1);
    }

    retour = sqlite3_bind_text(preparation_requete, 1, pseudo, -1, 0);
    retour |= sqlite3_bind_text(preparation_requete, 2, mot_de_passe, -1, 0);

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

int modifier_pseudo_compte(char *ancien, char *nouveau)
{
    sqlite3 *base_de_donnees = get_base_de_donnees();
    char sql[] = "UPDATE comptes SET pseudo = ? WHERE pseudo = ?;";
    sqlite3_stmt *preparation_requete = NULL;
    int retour = sqlite3_prepare_v2(base_de_donnees, sql, -1, &preparation_requete, NULL);
    if (retour != SQLITE_OK)
    {
        printf("Erreur : %s\n", sqlite3_errmsg(base_de_donnees));
        sqlite3_close(base_de_donnees);
        exit(1);
    }

    retour = sqlite3_bind_text(preparation_requete, 1, nouveau, -1, NULL);

    retour = sqlite3_bind_text(preparation_requete, 2, ancien, -1, NULL);

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
