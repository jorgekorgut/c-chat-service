
#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>
#include <errno.h>

#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include "serveur.h"
#include "communication.h"
#include "ecouteur_client.h"
#include "linkedlist_connexion.h"
#include "controleur_serveur.h"
#include "codes_constantes.h"

pthread_t *thread_ecouter_clients;
struct sockaddr_in *adresse_serveur;
size_t taille_adresse;
int descripteur_socket_serveur;


void initialiser_ecoute_clients(int descripteur_socket_serveur_t, struct sockaddr_in *adresse_serveur_t, size_t tailleAdresse_t)
{
    adresse_serveur = adresse_serveur_t;
    taille_adresse = tailleAdresse_t;
    descripteur_socket_serveur = descripteur_socket_serveur_t;
    thread_ecouter_clients = malloc(sizeof(pthread_t));

    if (listen(descripteur_socket_serveur, NUMBER_OF_CONNECTIONS) < 0)
    {
        perror("Impossible d'ecouter\n");
        exit(1);
    }

    int result = pthread_create(thread_ecouter_clients, NULL, ecouter_clients, NULL);

    if (result != 0)
    {
        perror("Erreur dans la creation du thread d'acceptation de clients.");
    }
    return;
}

void fermer_communication()
{
    shutdown(descripteur_socket_serveur, SHUT_RDWR);
    pthread_join(*thread_ecouter_clients, NULL);
    free(thread_ecouter_clients);
}

void *ecouter_clients()
{
    while (1)
    {
        socklen_t pair_addresse_connexion = sizeof(struct sockaddr_in);

        struct sockaddr_in *adresse_client = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
        bzero((void *)adresse_client, sizeof(struct sockaddr_in));
        adresse_client->sin_family = AF_INET;

        int descripteur_socket_client = accept(descripteur_socket_serveur, (struct sockaddr *)adresse_client, &pair_addresse_connexion);

        if (descripteur_socket_client == -1)
        {
            free(adresse_client);
            break;
        }

        connexion_client *connexion = (connexion_client *)malloc(sizeof(connexion_client));
        pthread_t *thread_connexion = (pthread_t *)malloc(sizeof(pthread_t));
        connexion->thread_client = thread_connexion;
        connexion->adresse = adresse_client;
        connexion->pseudo = NULL;
        char adresse_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(adresse_serveur->sin_addr), adresse_ip, INET_ADDRSTRLEN);
        connexion->descripteur_socket_client = descripteur_socket_client;
        linkedlist_connexion_client *liste_connexions = get_liste_connexions();
        ajouter_connexion_client(liste_connexions, connexion);

        printf("Connection of %s accepted\n", adresse_ip);
        int result = pthread_create(thread_connexion, NULL, gerer_connexion_client, (void *)(intptr_t)descripteur_socket_client);

        if (result != 0)
        {
            perror("Erreur dans la creation du thread d'ecoute d'un client.");
        }
    }

    pthread_exit(NULL);
}

void envoyer_message_client(int description_socket_client, char * message, int taille_message)
{
    write(description_socket_client, message, taille_message);
}