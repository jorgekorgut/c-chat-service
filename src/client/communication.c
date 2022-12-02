#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>
#include <stdint.h>

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "client.h"
#include "communication.h"
#include "ecouteur_serveur.h"

int descripteurSocket = -1;
pthread_t *thread_ecouter_serveur;

void initialiser_communication()
{
    thread_ecouter_serveur = (pthread_t *)malloc(sizeof(pthread_t));
    struct sockaddr_in adresseServeur;
    initialiser_parametres(PORT, ADRESSE_IP_SERVEUR, &adresseServeur, sizeof(adresseServeur));
    creer_socket_serveur();
    connecter_avec_serveur(&adresseServeur, sizeof(adresseServeur));
    creer_processus_ecoute();
}

void creer_processus_ecoute()
{
    pthread_create(thread_ecouter_serveur, NULL, gerer_ecoute_serveur, (void *)(intptr_t)descripteurSocket);
}

void fermer_communication()
{
    shutdown(descripteurSocket, SHUT_RDWR);
    fermer_socket(descripteurSocket);
    pthread_join(*thread_ecouter_serveur, NULL);
    free(thread_ecouter_serveur);
    descripteurSocket = -1;
}

void initialiser_parametres(int port, char *addresse, struct sockaddr_in *adresseServeur, size_t tailleAdresse)
{
    bzero((void *)adresseServeur, tailleAdresse);
    (*adresseServeur).sin_family = AF_INET;
    (*adresseServeur).sin_addr.s_addr = inet_addr(addresse);
    (*adresseServeur).sin_port = htons(port);
}

void creer_socket_serveur()
{
    descripteurSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (descripteurSocket < 0)
    {
        perror("Erreur de création de socket");
        exit(1);
    }
}

void connecter_avec_serveur(struct sockaddr_in *adresseServeur, size_t tailleAdresse)
{
    if (connect(descripteurSocket, (struct sockaddr *)adresseServeur, tailleAdresse) < 0)
    {
        perror("Erreur de connection à la socket");
        exit(1);
    }
}

void fermer_socket()
{
    if (close(descripteurSocket) < 0)
    {
        perror("Erreur de fermeture de la socket");
        exit(1);
    }
}

void ecrire_message_au_serveur(char *message, int taille_message)
{
    write(descripteurSocket, message, taille_message);
}