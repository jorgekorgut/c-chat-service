#include <sys/mman.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include "serveur.h"
#include "linkedlist_connexion.h"
#include "controleur_serveur.h"
#include "ecouteur_client.h"
#include "communication.h"

void *gerer_connexion_client(void *descripteur_socket_client_t)
{
    int descripteur_socket_client = (intptr_t)descripteur_socket_client_t;
    int sortir = 0;
    char message[TAILLE_MAX_MESSAGE];
    int taille_buffer = 50;
    char buffer[taille_buffer];

    linkedlist_connexion_client *liste_connexions = get_liste_connexions();
    connexion_client *connexion_actuelle = trouver_connexion_par_description_socket(liste_connexions, descripteur_socket_client);
    afficher_message_bien_venue(connexion_actuelle);
    while (sortir == 0)
    {
        ssize_t resultat = 1;
        int sortir_lecture = 0;
        message[0] = '\0';
        buffer[0] = '\0';
        while (sortir_lecture == 0)
        {
            resultat = recv(descripteur_socket_client, buffer, taille_buffer - 1, MSG_WAITFORONE);
            if (resultat == 0 || resultat == -1)
            {
                sortir_lecture = 1;
                sortir = 1;
            }
            else
            {
                buffer[resultat] = '\0';
                if (resultat < taille_buffer - 1)
                {
                    sortir_lecture = 1;
                }
                // TODO : Essayer de comprendre ? Porquoi il y a un \0 au debut d'un retour de read/recv ?
                if (buffer[0] == '\0')
                {
                    strcat(message, buffer + 1);
                }
                else
                {
                    strcat(message, buffer);
                }
            }
        }
        if (sortir != 1)
        {
            interpreter_message(connexion_actuelle, message);
        }
    }
    if (connexion_actuelle->pseudo == NULL)
    {
        printf("Utilisateur sans nom s'est deconnectee.\n");
    }
    else
    {
        printf("%s s'est deconnectee.\n", connexion_actuelle->pseudo);
    }

    fermer_ecouteur_client(connexion_actuelle);
    //pthread_detach(pthread_self()); // FIXME : Workaround, trouver une maniere de ecouter la fin de ce thread.
    pthread_exit(NULL);
}

void fermer_ecouteur_client(connexion_client *connexion)
{
    int descripteur_socket_client = connexion->descripteur_socket_client;
    linkedlist_connexion_client *liste_connexions = get_liste_connexions();
    enlever_connexion(liste_connexions, connexion);
    shutdown(descripteur_socket_client, SHUT_RDWR);
    close(descripteur_socket_client);
}

void afficher_erreur(connexion_client *connexion)
{
    char messsage_echec[] = "Une erreur s'est produite.";
    write(connexion->descripteur_socket_client, messsage_echec, strlen(messsage_echec));
}

void afficher_erreur_non_connectee(connexion_client *connexion)
{
    char message_renseigner_pseudo[] = "Vous n'etes pas encore connecte dans le chat, veillez se connecter!\n`/connecter <votre pseudo> <votre mot de passe>`";
    write(connexion->descripteur_socket_client, message_renseigner_pseudo, strlen(message_renseigner_pseudo));
}

void afficher_message_bien_venue(connexion_client *connexion)
{
    char message_bien_venue[] = "Bien venue sur le chat!\n\
        Pour creer un compte `/creer <votre pseudo> <votre mot de passe>`\n\
        Pour se connecter `/connecter <votre pseudo> <votre mot de passe>`";
    write(connexion->descripteur_socket_client, message_bien_venue, strlen(message_bien_venue));
}

void afficher_erreur_pseudo_existant(connexion_client *connexion)
{
    char messsage_changement_pseudo_echec[] = "Votre pseudo existe deja. Veillez choisir un nouveau.";
    write(connexion->descripteur_socket_client, messsage_changement_pseudo_echec, strlen(messsage_changement_pseudo_echec));
}

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
            else if (strstr(message, "/groupe") != NULL)
            {
                interpreter_commande_groupe(connexion, message);
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
        else if (strstr(message, "/groupe aide") != NULL)
        {
            interpreter_commande_aide_groupe(connexion);
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

// TODO : trouver une meilleure facon de recuperer les arguments.
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

void interpreter_commande_deconnexion(connexion_client *connexion)
{
    free(connexion->pseudo);
    char messsage_reussi[] = "Vous etes deconnectee du chat.";
    write(connexion->descripteur_socket_client, messsage_reussi, strlen(messsage_reussi));
    connexion->pseudo = NULL;
}

void interpreter_commande_creer(connexion_client *connexion, char *message)
{
    int descripteur_socket_client = connexion->descripteur_socket_client;
    char *pseudo = NULL;
    char *mot_de_passe = NULL;
    recuperer_arguments(message, 1, 3, &pseudo, &mot_de_passe);
    int etat_resultat = creer_un_compte(connexion, pseudo, mot_de_passe);

    if (etat_resultat == OK)
    {
        char messsage_reussi[] = "Vous etes enregistree. vous pouvez discuter dans le chat.";
        write(descripteur_socket_client, messsage_reussi, strlen(messsage_reussi));
    }
    else if (etat_resultat == ERREUR_SAISIE)
    {
        char messsage_echec[] = "Vous saisie est incorrecte.\n `/creer <compte> <mot de passe>`";
        write(descripteur_socket_client, messsage_echec, strlen(messsage_echec));
    }
    else if (etat_resultat == ERREUR_PSEUDO_EXISTANT)
    {
        afficher_erreur_pseudo_existant(connexion);
    }
    else if (etat_resultat == ERREUR_MOT_DE_PASSE_COURT)
    {
        char messsage_echec[] = "Votre mot de passe n'est pas assez long (superieur a 6 characteres).";
        write(descripteur_socket_client, messsage_echec, strlen(messsage_echec));
    }
    else
    {
        afficher_erreur(connexion);
    }

    free(pseudo);
    free(mot_de_passe);
}

void interpreter_commande_connexion(connexion_client *connexion, char *message)
{
    int descripteur_socket_client = connexion->descripteur_socket_client;
    char *pseudo = NULL;
    char *mot_de_passe = NULL;
    recuperer_arguments(message, 1, 3, &pseudo, &mot_de_passe);
    int etat_resultat = connecter_compte(connexion, pseudo, mot_de_passe);
    if (etat_resultat == OK)
    {
        char messsage_changement_pseudo_reussi[] = "Vous etes connectee.";
        write(descripteur_socket_client, messsage_changement_pseudo_reussi, strlen(messsage_changement_pseudo_reussi));
        envoyer_historique_au_compte(connexion);
    }
    else if (etat_resultat == ERREUR_MAUVAIS_MOT_DE_PASSE || etat_resultat == ERREUR_COMPTE_INEXISTANTE)
    {
        char messsage_mauvais_mot_de_passe[] = "Votre saisie pseudo/mot de passe est incorrecte.";
        write(descripteur_socket_client, messsage_mauvais_mot_de_passe, strlen(messsage_mauvais_mot_de_passe));
    }
    else
    {
        afficher_erreur(connexion);
    }

    free(pseudo);
    free(mot_de_passe);
}

void interpreter_commande_pseudo(connexion_client *connexion, char *message)
{
    int descripteur_socket_client = connexion->descripteur_socket_client;
    char *nouveau_pseudo = message + 8;
    size_t taille_pseudo = strlen(nouveau_pseudo);
    int etat_resultat = changer_pseudo(connexion, nouveau_pseudo, taille_pseudo);
    if (etat_resultat == OK)
    {
        char messsage_changement_pseudo_reussi[] = "Votre pseudo a été modifié.";
        write(descripteur_socket_client, messsage_changement_pseudo_reussi, strlen(messsage_changement_pseudo_reussi));
    }
    else if (etat_resultat == ERREUR_PSEUDO_EXISTANT)
    {
        afficher_erreur_pseudo_existant(connexion);
    }
    else
    {
        afficher_erreur(connexion);
    }
}

void interpreter_commande_privee(connexion_client *connexion, char *message)
{
    char *pseudo_recepteur = NULL;
    char *message_prive = NULL;
    recuperer_arguments(message, 1, 3, &pseudo_recepteur, &message_prive);
    int etat_resultat = envoyer_message_privee(connexion, message_prive, pseudo_recepteur);

    if (etat_resultat == OK){

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

// TODO : Faire groupe.
void interpreter_commande_groupe(connexion_client *connexion, char *message)
{
    char *pseudo_recepteur = NULL;
    char *message_prive = NULL;
    recuperer_arguments(message, 1, 3, &pseudo_recepteur, &message_prive);
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

    write(connexion->descripteur_socket_client, message_reponse, strlen(message_reponse));
}

void interpreter_commande_aide_groupe(connexion_client *connexion)
{
    char message_reponse[TAILLE_MAX_MESSAGE] = "-- Aide `groupe` --\n \
    `/groupe creer <nom> - pour creer un groupe.`\n\
    `/groupe <message> - pour envoyer une message dans votre groupe`\n\
    `/groupe rejoindre <nom>` pour rejoindre un groupe.\n\
    `/groupe sortir <nom> - pour sortir du groupe.` \n\
    `/groupe historique` - pour afficher l'historique des messages du groupe.";
    write(connexion->descripteur_socket_client, message_reponse, strlen(message_reponse));
}