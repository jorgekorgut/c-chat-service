#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreteur.h"
#include "interpreteur_compte.h"
#include "interpreteur_groupe.h"
#include "linkedlist_connexion.h"
#include "codes_constantes.h"
#include "codes_erreurs.h"
#include "communication.h"
#include "controleur_serveur_message.h"

void interpreter_message(connexion_client *connexion, char *message)
{
    printf("Message recu: %s\n", message);
    if (message[0] == '/')
    {
        if (connexion->pseudo == NULL)
        {
            if (strstr(message, "/creer") != NULL)
            {
                interpreter_commande_creer(connexion, message);
            }
            else if (strstr(message, "/connecter") != NULL)
            {
                interpreter_commande_connexion(connexion, message);
            }
            else
            {
                afficher_erreur_non_connectee(connexion);
            }
        }
        else
        {
            if (strstr(message, "/privee") != NULL)
            {
                interpreter_commande_privee(connexion, message);
            }
            else if (strstr(message, "/pseudo") != NULL)
            {
                interpreter_commande_pseudo(connexion, message);
            }
            else if (strstr(message, "/groupe creer") != NULL)
            {
                interpreter_commande_groupe_creer(connexion, message);
            }
            else if (strstr(message, "/groupe rejoindre") != NULL)
            {
                interpreter_commande_groupe_rejoindre(connexion, message);
            }
            else if (strstr(message, "/groupe sortir") != NULL)
            {
                interpreter_commande_groupe_sortir(connexion);
            }
            else if (strstr(message, "/groupe aide") != NULL)
            {
                interpreter_commande_aide_groupe(connexion);
            }
            else if (strstr(message, "/groupe") != NULL)
            {
                interpreter_commande_groupe_message(connexion, message);
            }
            else if (strstr(message, "/deconnecter") != NULL)
            {
                interpreter_commande_deconnexion(connexion);
            }
        }

        if (strstr(message, "/aide") != NULL)
        {
            interpreter_commande_aide(connexion);
        }
    }
    else
    {
        if (connexion->pseudo == NULL)
        {
            afficher_erreur_non_connectee(connexion);
        }
        else
        {
            envoyer_message_a_tous(connexion, message);
        }
    }
}

void interpreter_commande_privee(connexion_client *connexion, char *message)
{
    char *pseudo_recepteur = NULL;
    char *message_prive = NULL;
    recuperer_arguments(message, 1, 3, &pseudo_recepteur, &message_prive);
    int etat_resultat = envoyer_message_privee(connexion, message_prive, pseudo_recepteur);

    if (etat_resultat == OK)
    {
    }
    else if (etat_resultat == ERREUR_COMPTE_INEXISTANTE)
    {
        char reponse_personne_innexistante[] = "La personne que vous essayez de contacter n'existe pas.";
        envoyer_message_client(connexion->descripteur_socket_client, reponse_personne_innexistante, strlen(reponse_personne_innexistante));
    }
    else if (etat_resultat == ERREUR_PAS_CONNECTEE)
    {
        char reponse_personne_non_connectee[] = "La personne que vous essayez de contacter n'est pas connectee. Elle pourra voir votre message une fois connectee.";
        envoyer_message_client(connexion->descripteur_socket_client, reponse_personne_non_connectee, strlen(reponse_personne_non_connectee));
    }
    else
    {
        afficher_erreur(connexion);
    }

    free(pseudo_recepteur);
    free(message_prive);
}

void interpreter_commande_aide(connexion_client *connexion)
{
    char message_reponse[TAILLE_MAX_MESSAGE] = "-- Aide --\n \
    '/creer <votre pseudo>' <votre mot de passe> - pour creer un compte. \n\
    '/connecter <votre pseudo>' <votre mot de passe> - pour se connecter. \n\
    '/deconnecter <votre pseudo>' - pour se deconnecter. \n\
    `/pseudo <votre pseudo>` - pour changer de pseudo. \n\
    `/privee <pseudo> <message>` - pour envoyer des messages prives. \n\
    `/groupe aide - pour afficher les commandes relationne aux groupes.";

    envoyer_message_client(connexion->descripteur_socket_client, message_reponse, strlen(message_reponse));
}

void recuperer_arguments(char *message, int spaces_a_ignorer_inferieur, int ignorer_spaces_a_partir_de, char **premier_argument, char **deuxieme_argument)
{
    int iterateur = 0;
    int message_taille = strlen(message);
    int compter_espaces = 1;
    char *premier_argument_temporaire = NULL;
    char *deuxieme_argument_temporaire = NULL;
    while (message[iterateur] != '\0' && compter_espaces < ignorer_spaces_a_partir_de)
    {
        if (message[iterateur] == ' ')
        {
            if (spaces_a_ignorer_inferieur == compter_espaces)
            {
                premier_argument_temporaire = message + iterateur + 1;
            }
            else if (spaces_a_ignorer_inferieur + 1 == compter_espaces)
            {
                deuxieme_argument_temporaire = message + iterateur + 1;
            }
            compter_espaces++;
            message[iterateur] = '\0';
        }
        iterateur++;
    }

    if (premier_argument_temporaire == NULL || premier_argument_temporaire >= message + message_taille)
    {
        premier_argument_temporaire = message + message_taille;
    }

    if (deuxieme_argument_temporaire == NULL || deuxieme_argument_temporaire >= message + message_taille)
    {
        deuxieme_argument_temporaire = message + message_taille;
    }

    *premier_argument = (char *)malloc(sizeof(char) * strlen(premier_argument_temporaire) + 1);
    strcpy(*premier_argument, premier_argument_temporaire);

    *deuxieme_argument = (char *)malloc(sizeof(char) * strlen(deuxieme_argument_temporaire) + 1);
    strcpy(*deuxieme_argument, deuxieme_argument_temporaire);
}

void afficher_erreur(connexion_client *connexion)
{
    char messsage_echec[] = "Une erreur s'est produite.";
    envoyer_message_client(connexion->descripteur_socket_client, messsage_echec, strlen(messsage_echec));
}

void afficher_erreur_non_connectee(connexion_client *connexion)
{
    char message_renseigner_pseudo[] = "Vous n'etes pas encore connecte dans le chat, veillez se connecter!\n`/connecter <votre pseudo> <votre mot de passe>`";
    envoyer_message_client(connexion->descripteur_socket_client, message_renseigner_pseudo, strlen(message_renseigner_pseudo));
}

void afficher_message_bien_venue(connexion_client *connexion)
{
    char message_bien_venue[] = "Bien venue sur le chat!\n\
        Pour creer un compte `/creer <votre pseudo> <votre mot de passe>`\n\
        Pour se connecter `/connecter <votre pseudo> <votre mot de passe>`";
    envoyer_message_client(connexion->descripteur_socket_client, message_bien_venue, strlen(message_bien_venue));
}

void afficher_erreur_pseudo_existant(connexion_client *connexion)
{
    char messsage_changement_pseudo_echec[] = "Votre pseudo existe deja. Veillez choisir un nouveau.";
    envoyer_message_client(connexion->descripteur_socket_client, messsage_changement_pseudo_echec, strlen(messsage_changement_pseudo_echec));
}