#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include "serveur.h"
#include "controleur_serveur.h"
#include "linkedlist_connexion.h"
#include "communication.h"
#include "historique.h"

linkedlist_connexion_client *connexions;

void initialiser_controleur()
{
    connexions = (linkedlist_connexion_client *)malloc(sizeof(linkedlist_connexion_client));
    initialiser_linkedlist_connexions(connexions);
    initialiser_historique_messages();
}

linkedlist_connexion_client *get_liste_connexions()
{
    return connexions;
}

void fermer_controleur()
{
    fermer_historique_messages();
    fermer_linkedlist_connexions(connexions);
    free(connexions);
}

int demander_changement_pseudo(char *pseudo, int taille_pseudo)
{
    connexion_client *client_avec_meme_pseudo = trouver_connexion_par_pseudo(connexions, pseudo);

    return (client_avec_meme_pseudo == NULL) ? 1 : 0;
}

void formater_sous_groupe_message(char *groupe, char *message)
{
    strcat(message, "[");
    strcat(message, groupe);
    strcat(message, "]");
}

void formater_message(char *pseudo, char *groupe, char *destinataire, char *message, char *message_formatee)
{

    if (groupe != NULL)
    {
        formater_sous_groupe_message(groupe, message_formatee);
    }
    if (destinataire != NULL)
    {
        formater_sous_groupe_message("privee", message_formatee);
    }
    formater_sous_groupe_message(pseudo, message_formatee);
    strcat(message_formatee, " : ");
    strcat(message_formatee, message);
}

int ecrire_a_une_personne(connexion_client *emetteur, char *message, char *pseudo_recepteur)
{
    char message_formatee[TAILLE_MAX_MESSAGE] = "";
    connexion_client *recepteur = trouver_connexion_par_pseudo(connexions, pseudo_recepteur);

    if (recepteur == NULL) // Personne n'est pas connectee ou n'existe pas.
    {
        char reponse_personne_non_connectee[] = "La personne que vous essayez de contacter n'est pas connectee.";
        ecrire_message_au_client(emetteur->descripteur_socket_client, reponse_personne_non_connectee, strlen(reponse_personne_non_connectee));
        return 1;
    }
    else
    {
        information_message information_message;
        information_message.groupe = MESSAGE_PRIVE;
        information_message.emetteur = emetteur->pseudo;
        information_message.message = message;
        information_message.recepteur = pseudo_recepteur;
        ajouter_message(&information_message);

        formater_message(emetteur->pseudo, NULL, pseudo_recepteur, message, message_formatee);
        ecrire_message_au_client(recepteur->descripteur_socket_client, message_formatee, strlen(message_formatee));
    }
    return 0;
}

int ecrire_a_tous(connexion_client *emetteur, char *message)
{
    connexion_client *iterator = connexions->debut;
    while (iterator != NULL)
    {
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

    information_message information_message;
    information_message.groupe = MESSAGE_GLOBALE;
    information_message.emetteur = emetteur->pseudo;
    information_message.message = message;
    information_message.recepteur = PSEUDO_SERVEUR;
    ajouter_message(&information_message);

    return 0;
}