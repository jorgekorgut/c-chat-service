#include <stdlib.h>
#include <string.h>
#include "interpreteur.h"
#include "interpreteur_compte.h"
#include "controleur_serveur_message.h"
#include "codes_erreurs.h"
#include "communication.h"
#include "controleur_serveur_compte.h"

void interpreter_commande_deconnexion(connexion_client *connexion)
{
    free(connexion->pseudo);
    free(connexion->groupe);
    char messsage_reussi[] = "Vous etes deconnectee du chat.";
    envoyer_message_client(connexion->descripteur_socket_client, messsage_reussi, strlen(messsage_reussi));
    connexion->pseudo = NULL;
    connexion->groupe = NULL;
}

void interpreter_commande_creer(connexion_client *connexion, char *message)
{
    int descripteur_socket_client = connexion->descripteur_socket_client;
    char *pseudo = NULL;
    char *mot_de_passe = NULL;
    recuperer_arguments(message, 1, 3, &pseudo, &mot_de_passe);
    int etat_resultat = creer_un_compte(connexion, pseudo, mot_de_passe);

    if (etat_resultat == OK)
    {
        
        char messsage_reussi[] = "Vous etes enregistree. vous pouvez discuter dans le chat.";
        envoyer_message_client(descripteur_socket_client, messsage_reussi, strlen(messsage_reussi));
    }
    else if (etat_resultat == ERREUR_SAISIE)
    {
        char messsage_echec[] = "Vous saisie est incorrecte.\n `/creer <compte> <mot de passe>`";
        envoyer_message_client(descripteur_socket_client, messsage_echec, strlen(messsage_echec));
    }
    else if (etat_resultat == ERREUR_PSEUDO_EXISTANT)
    {
        afficher_erreur_pseudo_existant(connexion);
    }
    else if (etat_resultat == ERREUR_MOT_DE_PASSE_COURT)
    {
        char messsage_echec[] = "Votre mot de passe n'est pas assez long (superieur a 6 characteres).";
        envoyer_message_client(descripteur_socket_client, messsage_echec, strlen(messsage_echec));
    }
    else
    {
        afficher_erreur(connexion);
    }

    free(pseudo);
    free(mot_de_passe);
}

void interpreter_commande_connexion(connexion_client *connexion, char *message)
{
    int descripteur_socket_client = connexion->descripteur_socket_client;
    char *pseudo = NULL;
    char *mot_de_passe = NULL;
    recuperer_arguments(message, 1, 3, &pseudo, &mot_de_passe);
    int etat_resultat = connecter_compte(connexion, pseudo, mot_de_passe);
    if (etat_resultat == OK)
    {
        char messsage_changement_pseudo_reussi[] = "Vous etes connectee.";
        envoyer_message_client(descripteur_socket_client, messsage_changement_pseudo_reussi, strlen(messsage_changement_pseudo_reussi));
        envoyer_historique_au_compte(connexion);
    }
    else if (etat_resultat == ERREUR_MAUVAIS_MOT_DE_PASSE || etat_resultat == ERREUR_COMPTE_INEXISTANTE)
    {
        char messsage_echec[] = "Votre saisie pseudo/mot de passe est incorrecte.";
        envoyer_message_client(descripteur_socket_client, messsage_echec, strlen(messsage_echec));
    }
    else if(etat_resultat == ERREUR_COMPTE_CONNECTEE)
    {
        char messsage_echec[] = "Il existe deja un utilisateur connectee avec ce pseudo.";
        envoyer_message_client(descripteur_socket_client, messsage_echec, strlen(messsage_echec));
    }
    else
    {
        afficher_erreur(connexion);
    }

    free(pseudo);
    free(mot_de_passe);
}

void interpreter_commande_pseudo(connexion_client *connexion, char *message)
{
    int descripteur_socket_client = connexion->descripteur_socket_client;
    char *nouveau_pseudo = message + 8;
    size_t taille_pseudo = strlen(nouveau_pseudo);
    int etat_resultat = changer_pseudo(connexion, nouveau_pseudo, taille_pseudo);
    if (etat_resultat == OK)
    {
        char messsage_changement_pseudo_reussi[] = "Votre pseudo a été modifié.";
        envoyer_message_client(descripteur_socket_client, messsage_changement_pseudo_reussi, strlen(messsage_changement_pseudo_reussi));
    }
    else if (etat_resultat == ERREUR_PSEUDO_EXISTANT)
    {
        afficher_erreur_pseudo_existant(connexion);
    }
    else
    {
        afficher_erreur(connexion);
    }
}