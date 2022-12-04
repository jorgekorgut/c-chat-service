#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "controleur_serveur_groupe.h"
#include "controleur_serveur_message.h"
#include "interpreteur.h"
#include "interpreteur_groupe.h"
#include "codes_erreurs.h"
#include "codes_constantes.h"
#include "communication.h"

void interpreter_commande_groupe_message(connexion_client *connexion, char *message)
{
    if (connexion->groupe == NULL)
    {
        char reponse_erreur[] = "Vous n'etes pas membre d'un groupe.";
        envoyer_message_client(connexion->descripteur_socket_client, reponse_erreur, strlen(reponse_erreur));
        return;
    }

    char *message_groupe = NULL;
    char *argument_inutilisee = NULL;
    recuperer_arguments(message, 1, 2, &message_groupe, &argument_inutilisee);

    int resultat_etat = envoyer_message_groupe(connexion, connexion->groupe, message_groupe);

    if (resultat_etat != OK)
    {
        afficher_erreur(connexion);
    }

    free(message_groupe);
    free(argument_inutilisee);
}

void interpreter_commande_groupe_creer(connexion_client *connexion, char *message)
{
    char *groupe_nom = NULL;
    char *argument_inutilisee = NULL;
    recuperer_arguments(message, 2, 3, &groupe_nom, &argument_inutilisee);

    int resultat_etat = creer_groupe(connexion, groupe_nom);

    if (resultat_etat == OK)
    {
        char reponse_ok[] = "Vous avez cree le groupe [ ";
        strcat(reponse_ok, groupe_nom);
        strcat(reponse_ok, " ]");
        envoyer_message_client(connexion->descripteur_socket_client, reponse_ok, strlen(reponse_ok));
    }
    else if (resultat_etat == ERREUR_SAISIE)
    {
        char message_reponse[TAILLE_MAX_MESSAGE] = "Le nom du groupe doit etre plus grand que 6 characteres";
        envoyer_message_client(connexion->descripteur_socket_client, message_reponse, strlen(message_reponse));
    }
    else
    {
        afficher_erreur(connexion);
    }

    free(groupe_nom);
    free(argument_inutilisee);
}

void interpreter_commande_groupe_rejoindre(connexion_client *connexion, char *message)
{
    char *groupe_nom = NULL;
    char *argument_inutilisee = NULL;
    recuperer_arguments(message, 2, 3, &groupe_nom, &argument_inutilisee);

    int resultat_etat = rejoindre_groupe(connexion, groupe_nom);

    if (resultat_etat == OK)
    {
        char reponse_ok[] = "Vous avez rejoint le groupe [ ";
        strcat(reponse_ok, groupe_nom);
        strcat(reponse_ok, " ]");
        envoyer_message_client(connexion->descripteur_socket_client, reponse_ok, strlen(reponse_ok));
    }
    else
    {
        afficher_erreur(connexion);
    }

    free(groupe_nom);
}

void interpreter_commande_groupe_sortir(connexion_client *connexion)
{
    int resultat_etat = quitter_groupe(connexion);

    if (resultat_etat == OK)
    {
        char message_reponse[TAILLE_MAX_MESSAGE] = "Vous avez sortie de votre groupe.";
        envoyer_message_client(connexion->descripteur_socket_client, message_reponse, strlen(message_reponse));
    }
    else
    {
        afficher_erreur(connexion);
    }
}

void interpreter_commande_aide_groupe(connexion_client *connexion)
{
    char message_reponse[] = "-- Aide `groupe` --\n \
    `/groupe creer <nom> - pour creer un groupe.`\n\
    `/groupe <message> - pour envoyer une message dans votre groupe`\n\
    `/groupe rejoindre <nom>` - pour rejoindre un groupe.\n\
    `/groupe sortir - pour sortir du groupe.`";
    envoyer_message_client(connexion->descripteur_socket_client, message_reponse, strlen(message_reponse));
}