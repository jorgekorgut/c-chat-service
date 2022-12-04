#include <stdlib.h>
#include <string.h>
#include "controleur_serveur.h"
#include "controleur_serveur_message.h"
#include "linkedlist_connexion.h"
#include "linkedlist_information_message.h"
#include "codes_type_messages.h"
#include "codes_erreurs.h"
#include "acces_donnees_groupes.h"
#include "codes_constantes.h"

int rejoindre_groupe(connexion_client *connexion, char *groupe_nom)
{
    groupe *groupe_creation = chercher_groupe_par_nom(groupe_nom);
    if (groupe_creation == NULL)
    {
        fermer_groupe(groupe_creation);
        return ERREUR_GROUPE_INNEXISTANT;
    }

    modifier_groupe_compte(connexion->pseudo, groupe_nom);

    int groupe_nom_taille = strlen(groupe_nom);
    connexion->groupe = (char*)malloc(sizeof(char)*groupe_nom_taille+1);
    strcpy(connexion->groupe,groupe_nom);

    char message_entree_groupe[TAILLE_MAX_MESSAGE] = "L'utilisateur ";
    strcat(message_entree_groupe, connexion->pseudo);
    strcat(message_entree_groupe," viens de rejoindre votre groupe.");
    envoyer_message_groupe(connexion, groupe_nom, message_entree_groupe);

    fermer_groupe(groupe_creation);
    return OK;
}

int quitter_groupe(connexion_client *connexion)
{
    quitter_groupe_compte(connexion->pseudo);

    char message_groupe[TAILLE_MAX_MESSAGE] = "L'utilisateur ";
    strcat(message_groupe, connexion->pseudo);
    strcat(message_groupe," viens de sortir de votre groupe.");
    envoyer_message_groupe(connexion, connexion->groupe, message_groupe);

    free(connexion->groupe);
    connexion->groupe = NULL;
    return OK;
}

int creer_groupe(connexion_client *connexion, char *groupe_nom)
{
    groupe *groupe_creation = chercher_groupe_par_nom(groupe_nom);
    if (groupe_creation != NULL)
    {
        fermer_groupe(groupe_creation);
        return ERREUR_GROUPE_EXISTE_DEJA;
    }

    if (strlen(groupe_nom) < 5)
    {
        fermer_groupe(groupe_creation);
        return ERREUR_SAISIE;
    }

    ajouter_groupe(groupe_nom);

    modifier_groupe_compte(connexion->pseudo, groupe_nom);

    int groupe_nom_taille = strlen(groupe_nom);
    connexion->groupe = (char*)malloc(sizeof(char)*groupe_nom_taille+1);
    strcpy(connexion->groupe,groupe_nom);

    return OK;
}