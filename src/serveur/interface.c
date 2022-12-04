#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "interface.h"
#include "serveur.h"

void initialiser_interface()
{
    int sortir = 0;
    size_t taille_entree = 500;
    char *entree = (char *)malloc(sizeof(char) * taille_entree);
    afficher_ecoute_clients_pret();
    while (sortir == 0)
    {

        int result = demander_entree(&entree, &taille_entree);
        if (result == EOF)
        {
            sortir = 1;
            afficher_fin_application();
        }
        else
        {
            traiter_entree_serveur(entree);
        }
    }
    free(entree);
}

void traiter_entree_serveur(char *message)
{
    if (message[0] == '/')
    {
        if (strstr(message, "/aide") != NULL)
        {
            afficher_aide();
        }
        else if (strstr(message, "/groupe aide") != NULL)
        {
            afficher_groupe_aide();
        }
    }
    else
    {
        // TODO : Serveur doit pouvoir ecrire a tous aussi.
    }
}

int demander_entree(char **entree_client, size_t *taille_entree)
{
    int result = getline(entree_client, taille_entree, stdin);
    return result;
}

void afficher_ecoute_clients_pret()
{
    printf("Serveur bien inicialise. En attente de connexion sur le port %d\n", PORT);
}

void afficher_fin_application()
{
    printf("Le serveur s'etteint.\n");
}

void afficher_aide()
{
    char message[] = "-- Aide --\n \
    `/privee <pseudo> <message>` - pour envoyer des messages prives. \n\
    `/groupe aide - pour afficher les commandes relationne aux groupes.`\n\
    `/historique - pour afficher l'historique des messages.`";
    printf("%s\n", message);
}

void afficher_groupe_aide()
{
    char message[] = "-- Aide `groupe` --\n \
    `/groupe creer <nom> - pour creer un groupe.`\n\
    `/groupe <message> - pour envoyer une message dans votre groupe`\n\
    `/groupe rejoindre <nom>` pour rejoindre un groupe.\n\
    `/groupe sortir <nom> - pour sortir du groupe.` \n\
    `/groupe historique` - pour afficher l'historique des messages du groupe.";
    printf("%s\n", message);
}