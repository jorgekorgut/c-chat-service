#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "serveur.h"
#include "controleur_serveur.h"
#include "linkedlist_connexion.h"
#include "communication.h"
#include <sys/mman.h>

linkedlist_connexion_client *connexions;

void initialiser_controleur()
{
    connexions = (linkedlist_connexion_client *)malloc(sizeof(linkedlist_connexion_client));
    initialiser_linkedlist_connexions(connexions);
}

linkedlist_connexion_client *get_liste_connexions()
{
    return connexions;
}

void fermer_controleur()
{
    fermer_linkedlist_connexions(connexions);
    free(connexions);
}

int demander_changement_pseudo(char *pseudo, int taille_pseudo)
{
    connexion_client *client_avec_meme_pseudo = trouver_connexion_par_pseudo(connexions, pseudo);

    return (client_avec_meme_pseudo == NULL) ? 1 : 0;
}

int ecrire_a_tous(connexion_client *emetteur, char *message)
{
    connexion_client *iterator = connexions->debut;
    while (iterator != NULL)
    {
        printf("socket_client : %d\n", iterator->descripteur_socket_client);
        if (emetteur->descripteur_socket_client != iterator->descripteur_socket_client)
        {
            char message_formatee[TAILLE_MAX_MESSAGE] = "[";
            strcat(message_formatee, emetteur->pseudo);
            strcat(message_formatee, "] : ");
            strcat(message_formatee, message);
            int taille_message_formatee = strlen(message_formatee);
            ecrire_message_au_client(iterator->descripteur_socket_client, message_formatee, taille_message_formatee);
        }
        iterator = iterator->prochain;
    }
    return 1;
}