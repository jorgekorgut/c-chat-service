#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "serveur.h"
#include "communication.h"
#include "interface.h"

int descripteur_socket = -1;

void initialiser_parametres_socket(int port, struct sockaddr_in *adresse_serveur, size_t tailleAdresse)
{
    bzero((void *)adresse_serveur, tailleAdresse);

    (*adresse_serveur).sin_family = AF_INET;
    (*adresse_serveur).sin_addr.s_addr = htonl(INADDR_ANY);
    (*adresse_serveur).sin_port = htons(port);
}

int creer_socket()
{
    descripteur_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (descripteur_socket < 0)
    {
        perror("Erreur de création de socket");
        exit(1);
    }
    return descripteur_socket;
}

void fermer_socket()
{
    shutdown(descripteur_socket,SHUT_RDWR);
    if (close(descripteur_socket) < 0)
    {
        perror("Erreur de fermeture de la socket");
        exit(1);
    }
}

/*
 * Le bind sert a indiquer au système la façon dont il doit accepter les connections de la part d’éventuels clients.
 */
void lier_socket(struct sockaddr_in *adresse_serveur, size_t tailleAdresse)
{
    if (bind(descripteur_socket, (struct sockaddr *)adresse_serveur, tailleAdresse) < 0)
    {
        perror("Impossible de faire le bind\n");
        exit(0);
    }
}

int initialiser_serveur()
{
    struct sockaddr_in adresse_serveur;
    initialiser_parametres_socket(PORT, &adresse_serveur, sizeof(adresse_serveur));
    descripteur_socket = creer_socket();
    lier_socket(&adresse_serveur, sizeof(adresse_serveur));
    initialiser_ecoute_clients(descripteur_socket, &adresse_serveur, sizeof(adresse_serveur));
    initialiser_interface();
    return 0;
}

void fermer_serveur()
{
    fermer_communication();
    fermer_socket(descripteur_socket);
}