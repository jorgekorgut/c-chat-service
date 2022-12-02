#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include "ecouteur_serveur.h"
#include "communication.h"

int descripteur_socket_serveur = -1;

void *gerer_ecoute_serveur(void *descripteur_socket_serveur_t)
{
    descripteur_socket_serveur = (intptr_t)descripteur_socket_serveur_t;
    int sortir = 0;
    char message[500];
    int taille_buffer = 50;
    char buffer[taille_buffer];

    while (sortir == 0)
    {
        ssize_t resultat = 1;
        int sortir_lecture = 0;
        message[0] = '\0';
        buffer[0] = '\0';
        while (sortir_lecture == 0)
        {
            resultat = recv(descripteur_socket_serveur, buffer, taille_buffer - 1, MSG_WAITFORONE);
            if (resultat == 0 || resultat == -1)
            {
                sortir_lecture = 1;
                sortir = 1;
                break;
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
            interpreter_message(message);
        }
    }
    pthread_exit(NULL);
}

void interpreter_message(char *message)
{
    printf("%s\n", message);
}