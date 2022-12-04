#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "communication.h"
#include "interface.h"

void initialiser_interface()
{
    size_t taille_entree = 500;
    char * entree_client = (char * )malloc(sizeof(char)*taille_entree);
    int sortir = 0;
    while (sortir == 0)
    {
        int result = demander_entree(&entree_client, &taille_entree);
        if (result == EOF)
        {
            sortir = 1;
            printf("Deconnexion.\n");
        }
        else
        {
            traiter_entree_utilisateur(entree_client);
        }
    }
    free(entree_client);
}

int demander_entree(char **entree_client, size_t *taille_entree)
{
    int result = getline(entree_client, taille_entree, stdin);
    return result;
}

void traiter_entree_utilisateur(char *entree_client)
{
    int taille_message = strlen(entree_client);  
    enlever_saut_ligne(entree_client, taille_message);
    ecrire_message_au_serveur(entree_client, taille_message);
}

void enlever_saut_ligne(char *message, int taille_message)
{
    for (int i = 0; i < taille_message; i++)
    {
        if (message[i] == '\n')
        {
            message[i] = '\0';
        }
    }
}
