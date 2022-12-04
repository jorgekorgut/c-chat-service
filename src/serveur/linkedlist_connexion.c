#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linkedlist_connexion.h"
#include "serveur.h"

int initialiser_linkedlist_connexions(linkedlist_connexion_client *liste)
{
    liste->debut = NULL;
    liste->taille = 0;
    return 1;
}

int ajouter_connexion_client(linkedlist_connexion_client *liste, connexion_client *connexion)
{
    connexion->prochain = liste->debut;
    connexion->ancient = NULL;
    if (liste->debut != NULL)
    {
        liste->debut->ancient = connexion;
    }
    liste->debut = connexion;
    liste->taille++;
    return 1;
}

connexion_client *trouver_connexion_par_description_socket(linkedlist_connexion_client *liste, int description_socket_client)
{
    connexion_client *iterator = liste->debut;

    while (iterator != NULL)
    {
        if (iterator->descripteur_socket_client == description_socket_client)
        {
            break;
        }
        iterator = iterator->prochain;
    }
    return iterator;
}

connexion_client *trouver_connexion_par_pseudo(linkedlist_connexion_client *liste, char *pseudo)
{
    connexion_client *iterator = liste->debut;

    while (iterator != NULL)
    {
        if (iterator->pseudo != NULL)
        {   
            if (strcmp(iterator->pseudo, pseudo) == 0)
            {
                break;
            }
        }
        iterator = iterator->prochain;
    }

    return iterator;
}

int enlever_connexion_par_description_socket(linkedlist_connexion_client *liste, int descripteur_socket){
    return enlever_connexion(liste, trouver_connexion_par_description_socket(liste, descripteur_socket));
}

int enlever_connexion(linkedlist_connexion_client *liste, connexion_client *enlever)
{
    if(enlever == NULL){
        return 0;
    }

    if (enlever->prochain != NULL)
    {
        enlever->prochain->ancient = enlever->ancient;
    }
    if (enlever->ancient != NULL)
    {
        enlever->ancient->prochain = enlever->prochain;
    }
    else
    {
        liste->debut = enlever->prochain;
    }

    fermer_connexion(enlever);
    liste->taille --;

    return 1;
}

int fermer_connexion(connexion_client *eliminer)
{
    fermer_socket(eliminer->descripteur_socket_client);
    pthread_join(*eliminer->thread_client, NULL);
    free(eliminer->thread_client);
    free(eliminer->adresse);
    free(eliminer->pseudo);
    free(eliminer->groupe);
    free(eliminer);

    return 1;
}

int fermer_linkedlist_connexions(linkedlist_connexion_client *liste)
{
    connexion_client *iterator = liste->debut;

    while (iterator != NULL)
    {
        connexion_client *eliminer = iterator;
        iterator = iterator->prochain;
        fermer_connexion(eliminer);
    }
    liste->debut = NULL;
    liste->taille = 0;

    return 1;
}