#include <stdio.h>
#include <string.h>
#include "controleur_serveur.h"
#include "controleur_serveur_message.h"
#include "codes_type_messages.h"
#include "codes_erreurs.h"
#include "codes_constantes.h"
#include "acces_donnees_comptes.h"
#include "acces_donnees_messages.h"
#include "linkedlist_information_message.h"
#include "communication.h"

void formater_sous_groupe_message(char *groupe, char *message)
{
    strcat(message, "[");
    strcat(message, groupe);
    strcat(message, "]");
}

void formater_date(time_t date, char *message)
{
    char date_formatee[50];
    struct tm *timeinfo;
    timeinfo = localtime(&date);

    sprintf(date_formatee, "%d/%d/%d %d:%d", timeinfo->tm_mday,
            timeinfo->tm_mon + 1, timeinfo->tm_year + 1900,
            timeinfo->tm_hour, timeinfo->tm_min);

    strcat(message, "<");
    strcat(message, date_formatee);
    strcat(message, ">");
}

void formater_message(char *pseudo, char *type_message, char *groupe, char *destinataire, char *message, char *message_formatee, time_t date)
{
    formater_date(date, message_formatee);
    formater_sous_groupe_message(type_message, message_formatee);
    if (strcmp(type_message, MESSAGE_PRIVE) == 0)
    {
        formater_sous_groupe_message(pseudo, message_formatee);
        strcat(message_formatee, "->");
        formater_sous_groupe_message(destinataire, message_formatee);
    }
    else if (strcmp(type_message, MESSAGE_GROUPE) == 0)
    {
        formater_sous_groupe_message(groupe, message_formatee);
        formater_sous_groupe_message(pseudo, message_formatee);
    }
    else
    {
        formater_sous_groupe_message(pseudo, message_formatee);
    }

    strcat(message_formatee, " : ");
    strcat(message_formatee, message);
}

int envoyer_message_privee(connexion_client *connexion, char *message, char *pseudo_recepteur)
{
    linkedlist_connexion_client * connexions = get_liste_connexions();
    compte *recepteur_base_de_donnees = chercher_compte_par_pseudo(pseudo_recepteur);
    if (recepteur_base_de_donnees == NULL)
    {
        fermer_compte(recepteur_base_de_donnees);
        return ERREUR_COMPTE_INEXISTANTE;
    }

    char message_formatee[TAILLE_MAX_MESSAGE] = "";

    connexion_client *recepteur_connexion = trouver_connexion_par_pseudo(connexions, pseudo_recepteur);
    if (recepteur_connexion == NULL)
    {
        fermer_compte(recepteur_base_de_donnees);
        return ERREUR_PAS_CONNECTEE;
    }

    if (strcmp(connexion->pseudo, pseudo_recepteur) == 0)
    {
        fermer_compte(recepteur_base_de_donnees);
        return ERREUR_COMPTE_PROPRE;
    }

    information_message information_message;
    information_message.groupe_nom = NULL;
    information_message.type = MESSAGE_PRIVE;
    information_message.emetteur = connexion->pseudo;
    information_message.message = message;
    information_message.recepteur = pseudo_recepteur;
    ajouter_message(&information_message);

    formater_message(connexion->pseudo, MESSAGE_PRIVE, NULL, pseudo_recepteur, message, message_formatee, information_message.date_envoye);
    envoyer_message_client(recepteur_connexion->descripteur_socket_client, message_formatee, strlen(message_formatee));

    fermer_compte(recepteur_base_de_donnees);
    return OK;
}

int envoyer_message_a_tous(connexion_client *connexion, char *message)
{
    linkedlist_connexion_client * connexions = get_liste_connexions();

    if (connexion->pseudo == NULL)
    {
        return ERREUR_PAS_CONNECTEE;
    }

    information_message information_message;
    information_message.groupe_nom = NULL;
    information_message.type = MESSAGE_GLOBALE;
    information_message.emetteur = connexion->pseudo;
    information_message.message = message;
    information_message.recepteur = NULL;
    ajouter_message(&information_message);

    connexion_client *iterator = connexions->debut;
    while (iterator != NULL)
    {
        if (connexion->descripteur_socket_client != iterator->descripteur_socket_client)
        {
            char message_formatee[TAILLE_MAX_MESSAGE] = "";
            formater_message(connexion->pseudo, information_message.type, information_message.groupe_nom, information_message.recepteur, information_message.message, message_formatee, information_message.date_envoye);
            int taille_message_formatee = strlen(message_formatee);
            envoyer_message_client(iterator->descripteur_socket_client, message_formatee, taille_message_formatee);
        }
        iterator = iterator->prochain;
    }

    return OK;
}

int envoyer_message_groupe(connexion_client *connexion, char *groupe_nom, char *message)
{
    linkedlist_connexion_client * connexions = get_liste_connexions();
    if (connexion->pseudo == NULL)
    {
        return ERREUR_PAS_CONNECTEE;
    }

    information_message information_message;
    information_message.groupe_nom = groupe_nom;
    information_message.type = MESSAGE_GROUPE;
    information_message.emetteur = connexion->pseudo;
    information_message.message = message;
    information_message.recepteur = NULL;
    ajouter_message(&information_message);

    connexion_client *iterator = connexions->debut;
    while (iterator != NULL)
    {
        if (connexion->groupe != NULL && iterator->groupe != NULL)
        {
            printf("%s|%s",iterator->groupe,connexion->groupe);
            if (strcmp(connexion->groupe, iterator->groupe) == 0)
            {
                if (connexion->descripteur_socket_client != iterator->descripteur_socket_client)
                {
                    char message_formatee[TAILLE_MAX_MESSAGE] = "";
                    formater_message(information_message.emetteur, information_message.type, information_message.groupe_nom, NULL, information_message.message, message_formatee, information_message.date_envoye);
                    
                    int taille_message_formatee = strlen(message_formatee);
                    envoyer_message_client(iterator->descripteur_socket_client, message_formatee, taille_message_formatee);
                }
            }
        }

        iterator = iterator->prochain;
    }

    return OK;
}

int envoyer_historique_au_compte(connexion_client *connexion)
{
    linkedlist_information_message *liste_messages = chercher_historique_messages_pseudo(connexion->pseudo);
    char message_formatee[TAILLE_MAX_MESSAGE] = "";

    information_message *iterateur = liste_messages->debut;
    while (iterateur != NULL)
    {
        formater_message(iterateur->emetteur, iterateur->type, iterateur->groupe_nom, iterateur->recepteur, iterateur->message, message_formatee, iterateur->date_envoye);
        message_formatee[strlen(message_formatee)] = '\n';
        iterateur = iterateur->prochain;
    }
    message_formatee[strlen(message_formatee) - 1] = '\0';
    printf("%s\n", message_formatee);

    envoyer_message_client(connexion->descripteur_socket_client, message_formatee, strlen(message_formatee));
    fermer_linkedlist_information_message(liste_messages);
    return 1;
}