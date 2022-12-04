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
#include "codes_constantes.h"
#include "codes_erreurs.h"
#include "interpreteur.h"

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

    if (connexion_actuelle != NULL)
    {
        pthread_detach(pthread_self()); // FIXME : Workaround, trouver une maniere de ecouter la fin de ce thread.
        fermer_ecouteur_client(connexion_actuelle);
    }

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