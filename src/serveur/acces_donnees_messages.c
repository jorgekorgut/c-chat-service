#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "base_de_donnees.h"
#include "linkedlist_information_message.h"
#include "acces_donnees_comptes.h"
#include "acces_donnees_groupes.h"
#include "codes_erreurs.h"
#include "codes_type_messages.h"

linkedlist_information_message *chercher_historique_messages_pseudo(char *pseudo)
{
    sqlite3 *base_de_donnees = get_base_de_donnees();
    linkedlist_information_message *liste_information_messages = (linkedlist_information_message *)malloc(sizeof(linkedlist_information_message));
    initialiser_linkedlist_information_message(liste_information_messages);

    compte *compte_a_chercher = chercher_compte_par_pseudo(pseudo);
    if (compte_a_chercher == NULL)
    {
        return ERREUR;
    }

    char sql[] = "SELECT\
                    h.id_message,\
                    g.nom,\
                    c1.pseudo,\
                    c2.pseudo,\
                    h.message,\
                    h.date_envoye,\
                    h.type_message\
                FROM\
                    historique_messages h,\
                    comptes c1\
                    LEFT JOIN groupes g ON\
                        h.id_groupe = g.id_groupe\
                    LEFT JOIN comptes c2 ON\
                        h.id_recepteur = c2.id_compte\
                WHERE\
                    (h.id_emetteur = c1.id_compte) AND\
                    (\
                        (h.type_message = 'globale') OR\
                        (\
                            ( (c1.pseudo = ?) OR (c2.pseudo = ?) ) AND h.type_message = 'privee'\
                        ) OR\
                        (h.id_groupe = ?)\
                    )\
                ORDER BY \
                    h.date_envoye DESC;";

    sqlite3_stmt *preparation_requete = NULL;
    int retour = sqlite3_prepare_v2(base_de_donnees, sql, -1, &preparation_requete, NULL);
    if (retour != SQLITE_OK)
    {
        printf("Erreur : %s\n", sqlite3_errmsg(base_de_donnees));
        sqlite3_close(base_de_donnees);
        exit(1);
    }

    retour = sqlite3_bind_text(preparation_requete, 1, compte_a_chercher->pseudo, -1, 0);
    retour = sqlite3_bind_text(preparation_requete, 2, compte_a_chercher->pseudo, -1, 0);
    retour = sqlite3_bind_int(preparation_requete, 3, compte_a_chercher->groupe);

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
            information_message *info_message = (information_message *)malloc(sizeof(information_message));
            info_message->groupe_nom = NULL;
            info_message->recepteur = NULL;

            info_message->id = (int)sqlite3_column_int(preparation_requete, 0);

            char *groupe_base_de_donees = (char *)sqlite3_column_text(preparation_requete, 1);
            if (groupe_base_de_donees != NULL)
            {
                info_message->groupe_nom = (char *)malloc(sizeof(char) * (strlen(groupe_base_de_donees) + 1));
                strcpy(info_message->groupe_nom, groupe_base_de_donees);
            }

            char *emmeteur_base_de_donees = (char *)sqlite3_column_text(preparation_requete, 2);
            info_message->emetteur = (char *)malloc(sizeof(char) * (strlen(emmeteur_base_de_donees) + 1));
            strcpy(info_message->emetteur, emmeteur_base_de_donees);

            if (sqlite3_column_type(preparation_requete, 3) != SQLITE_NULL)
            {
                char *recepteur_base_de_donees = (char *)sqlite3_column_text(preparation_requete, 3);
                info_message->recepteur = (char *)malloc(sizeof(char) * strlen(recepteur_base_de_donees) + 1);
                strcpy(info_message->recepteur, recepteur_base_de_donees);
            }

            char *message_base_de_donees = (char *)sqlite3_column_text(preparation_requete, 4);
            info_message->message = (char *)malloc(sizeof(char) * (strlen(message_base_de_donees) + 1));
            strcpy(info_message->message, message_base_de_donees);

            info_message->date_envoye = (int)sqlite3_column_int(preparation_requete, 5);

            char *type_message_base_de_donees = (char *)sqlite3_column_text(preparation_requete, 6);
            info_message->type = (char *)malloc(sizeof(char) * (strlen(type_message_base_de_donees) + 1));
            strcpy(info_message->type, type_message_base_de_donees);

            ajouter_information_message(liste_information_messages, info_message);
        }

        if (retour == SQLITE_DONE)
        {
            break;
        }
    }

    fermer_compte(compte_a_chercher);
    fermer_preparation_requete(preparation_requete);
    return liste_information_messages;
}

int ajouter_message(information_message *message)
{
    sqlite3 *base_de_donnees = get_base_de_donnees();
    message->date_envoye = time(0);
    compte *recepteur = NULL;
    compte *emetteur = chercher_compte_par_pseudo(message->emetteur);
    groupe *groupe_cherche = chercher_groupe_par_nom(message->groupe_nom);
    if (message->recepteur != NULL)
    {
        recepteur = chercher_compte_par_pseudo(message->recepteur);
    }

    char sql[] = "INSERT INTO historique_messages VALUES(NULL, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *preparation_requete = NULL;
    int retour = sqlite3_prepare_v2(base_de_donnees, sql, -1, &preparation_requete, NULL);
    if (retour != SQLITE_OK)
    {
        printf("Erreur : %s\n", sqlite3_errmsg(base_de_donnees));
        sqlite3_close(base_de_donnees);
        exit(1);
    }

    if (groupe_cherche != NULL)
    {
        retour = sqlite3_bind_int(preparation_requete, 1, groupe_cherche->id);
    }
    else
    {
        retour = sqlite3_bind_null(preparation_requete, 1);
    }

    retour |= sqlite3_bind_int(preparation_requete, 2, emetteur->id);

    if (strcmp(message->type, MESSAGE_PRIVE) == 0)
    {
        retour |= sqlite3_bind_int(preparation_requete, 3, recepteur->id);
    }
    else
    {
        retour |= sqlite3_bind_null(preparation_requete, 3);
    }

    retour |= sqlite3_bind_text(preparation_requete, 4, message->message, -1, 0);

    retour |= sqlite3_bind_int64(preparation_requete, 5, message->date_envoye);

    retour |= sqlite3_bind_text(preparation_requete, 6, message->type, -1, 0);

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

    fermer_compte(emetteur);
    fermer_groupe(groupe_cherche);
    fermer_compte(recepteur);

    fermer_preparation_requete(preparation_requete);
    return 1;
}