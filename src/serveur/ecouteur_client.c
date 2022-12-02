#include <sys/mman.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
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
    printf("Utilisateur deconnectee.\n");
    fermer_ecouteur_client(connexion_actuelle);
    pthread_detach(pthread_self()); // FIXME : Workaround, trouver une maniere de ecouter la fin de ce thread.
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

void demander_pseudo(connexion_client *connexion)
{
    char *choix_pseudo = "Veillez choisir votre pseudo. `/pseudo <votre_pseudo>`";
    write(connexion->descripteur_socket_client, choix_pseudo, strlen(choix_pseudo));
}

void interpreter_message(connexion_client *connexion, char *message)
{
    int descripteur_socket_client = connexion->descripteur_socket_client;
    printf("Message recu: %s\n", message);
    if (message[0] == '/')
    {
        if (strcmp(message, "/pseudo"))
        {
            char *pseudo_incertain = message + 8;
            size_t taille_pseudo = strlen(pseudo_incertain);
            int changement_reussi = demander_changement_pseudo(pseudo_incertain, taille_pseudo);
            if (changement_reussi)
            {
                char *pseudo = (char *)malloc(sizeof(char) * (taille_pseudo + 1));
                strcpy(pseudo, pseudo_incertain);

                connexion->pseudo = pseudo;
                char messsage_changement_pseudo_reussi[] = "Votre pseudo a été modifié.";
                write(descripteur_socket_client, messsage_changement_pseudo_reussi, strlen(messsage_changement_pseudo_reussi));
            }
            else
            {
                char messsage_changement_pseudo_echec[] = "Votre pseudo existe deja. Veillez choisir un nouveau.";
                write(descripteur_socket_client, messsage_changement_pseudo_echec, strlen(messsage_changement_pseudo_echec));
            }
        }

        if(strcmp(message, "/aide") == 0)
        {
            char message_reponse[TAILLE_MAX_MESSAGE] = "-- Aide --\n \
                                                        `/pseudo <votre pseudo>` - pour changer de pseudo. \n\
                                                        `/prive <pseudo> <message>` - pour envoyer des messages prives. ";

            write(descripteur_socket_client, message_reponse, strlen(message_reponse));                                        
        }
    }
    else
    {
        if (connexion->pseudo == NULL)
        {
            char message_renseigner_pseudo[] = "Vous ne pouvez pas ecrire dans le chat, veillez renseigner votre pseudo!\n`/pseudo <votre pseudo>`";
            write(descripteur_socket_client, message_renseigner_pseudo, strlen(message_renseigner_pseudo));
        }
        else
        {
            ecrire_a_tous(connexion, message);
        }
    }
}