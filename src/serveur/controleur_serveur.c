#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include "serveur.h"
#include "controleur_serveur.h"
#include "linkedlist_connexion.h"
#include "communication.h"
#include "base_de_donnees.h"
#include "linkedlist_information_message.h"

linkedlist_connexion_client *connexions;

void initialiser_controleur()
{
    connexions = (linkedlist_connexion_client *)malloc(sizeof(linkedlist_connexion_client));
    initialiser_linkedlist_connexions(connexions);
    initialiser_historique_messages();
}

linkedlist_connexion_client *get_liste_connexions()
{
    return connexions;
}

void fermer_controleur()
{
    fermer_historique_messages();
    fermer_linkedlist_connexions(connexions);
    free(connexions);
}

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

    else
    {
        information_message information_message;
        information_message.groupe_nom = NULL;
        information_message.type = MESSAGE_PRIVE;
        information_message.emetteur = connexion->pseudo;
        information_message.message = message;
        information_message.recepteur = pseudo_recepteur;
        ajouter_message(&information_message);

        formater_message(connexion->pseudo, MESSAGE_PRIVE, NULL, pseudo_recepteur, message, message_formatee, information_message.date_envoye);
        envoyer_message_client(recepteur_connexion->descripteur_socket_client, message_formatee, strlen(message_formatee));
    }

    fermer_compte(recepteur_base_de_donnees);
    return OK;
}

int envoyer_message_a_tous(connexion_client *connexion, char *message)
{
    if (connexion->pseudo == NULL)
    {
        return ERREUR_PAS_CONNECTEE;
    }

    information_message information_message;
    information_message.groupe_nom = NULL;
    information_message.type = MESSAGE_GLOBALE;
    information_message.emetteur = connexion->pseudo;
    information_message.message = message;
    information_message.recepteur = PSEUDO_SERVEUR;
    ajouter_message(&information_message);

    connexion_client *iterator = connexions->debut;
    while (iterator != NULL)
    {
        if (connexion->descripteur_socket_client != iterator->descripteur_socket_client)
        {
            char message_formatee[TAILLE_MAX_MESSAGE] = "";
            formater_message(connexion->pseudo, MESSAGE_GLOBALE, NULL, NULL, message, message_formatee, information_message.date_envoye);
            int taille_message_formatee = strlen(message_formatee);
            envoyer_message_client(iterator->descripteur_socket_client, message_formatee, taille_message_formatee);
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

int creer_un_compte(connexion_client *connexion, char *pseudo, char *mot_de_passe)
{
    if (strlen(pseudo) == 0 || strlen(mot_de_passe) == 0)
    {
        return ERREUR_SAISIE;
    }

    compte *compte_cherchee = chercher_compte_par_pseudo(pseudo);

    if (compte_cherchee != NULL)
    {
        fermer_compte(compte_cherchee);
        return ERREUR_PSEUDO_EXISTANT;
    }

    if (strlen(mot_de_passe) < 5)
    {
        fermer_compte(compte_cherchee);
        return ERREUR_MOT_DE_PASSE_COURT;
    }

    ajouter_compte(pseudo, mot_de_passe);
    connexion->pseudo = malloc(sizeof(char) * strlen(pseudo) + 1);
    strcpy(connexion->pseudo, pseudo);

    fermer_compte(compte_cherchee);
    return OK;
}

int connecter_compte(connexion_client *connexion, char *pseudo, char *mot_de_passe)
{
    compte *compte_cherchee = chercher_compte_par_pseudo(pseudo);

    if (compte_cherchee == NULL)
    {
        fermer_compte(compte_cherchee);
        return ERREUR_COMPTE_INEXISTANTE;
    }

    // TODO Ajouter fonction de cryptage
    char *mot_de_passe_renseigne = mot_de_passe;

    char *mot_de_passe_base_de_donnees = compte_cherchee->mot_de_passe;

    if (strcmp(mot_de_passe_renseigne, mot_de_passe_base_de_donnees) != 0)
    {
        free(compte_cherchee);
        return ERREUR_MAUVAIS_MOT_DE_PASSE;
    }

    connexion->pseudo = malloc(sizeof(char) * strlen(pseudo) + 1);
    strcpy(connexion->pseudo, pseudo);

    fermer_compte(compte_cherchee);
    return OK;
}

int changer_pseudo(connexion_client *connexion, char *nouveau_pseudo, int taille_pseudo)
{
    if (connexion->pseudo == NULL)
    {
        return ERREUR_PAS_CONNECTEE;
    }

    compte *compte_meme_pseudo = chercher_compte_par_pseudo(nouveau_pseudo);

    if (compte_meme_pseudo != NULL)
    {
        return ERREUR_PSEUDO_EXISTANT;
    }

    int resultat_changement_pseudo = modifier_pseudo_compte(connexion->pseudo, nouveau_pseudo);

    if (resultat_changement_pseudo == OK)
    {
        char *pseudo = (char *)malloc(sizeof(char) * (taille_pseudo + 1));
        strcpy(pseudo, nouveau_pseudo);
        free(connexion->pseudo);
        connexion->pseudo = pseudo;
    }

    return resultat_changement_pseudo;
}