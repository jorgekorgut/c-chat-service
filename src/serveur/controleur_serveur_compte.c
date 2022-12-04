#include <string.h>
#include <stdlib.h>
#include "linkedlist_connexion.h"
#include "controleur_serveur.h"
#include "controleur_serveur_compte.h"
#include "codes_erreurs.h"
#include "acces_donnees_comptes.h"
#include "acces_donnees_groupes.h"
#include "chiffrement.h"

int creer_un_compte(connexion_client *connexion, char *pseudo, char *mot_de_passe)
{
    if (strlen(pseudo) == 0 || strlen(mot_de_passe) == 0)
    {
        return ERREUR_SAISIE;
    }

    compte *compte_cherchee = chercher_compte_par_pseudo(pseudo);

    if (compte_cherchee != NULL)
    {
        fermer_compte(compte_cherchee);
        return ERREUR_PSEUDO_EXISTANT;
    }

    if (strlen(mot_de_passe) < 5)
    {
        fermer_compte(compte_cherchee);
        return ERREUR_MOT_DE_PASSE_COURT;
    }

    char * mot_de_passe_chiffre = chiffrer_mot_de_passe(mot_de_passe);

    ajouter_compte(pseudo, mot_de_passe_chiffre);
    connexion->pseudo = malloc(sizeof(char) * strlen(pseudo) + 1);
    strcpy(connexion->pseudo, pseudo);
    connexion->groupe = NULL;

    fermer_chiffrement(mot_de_passe_chiffre);
    fermer_compte(compte_cherchee);
    return OK;
}

int connecter_compte(connexion_client *connexion, char *pseudo, char *mot_de_passe)
{
    linkedlist_connexion_client * connexions = get_liste_connexions();
    compte *compte_cherchee = chercher_compte_par_pseudo(pseudo);

    if (compte_cherchee == NULL)
    {
        fermer_compte(compte_cherchee);
        return ERREUR_COMPTE_INEXISTANTE;
    }

    connexion_client *compte_connectee = trouver_connexion_par_pseudo(connexions, pseudo);

    if (compte_connectee != NULL)
    {
        fermer_compte(compte_cherchee);
        return ERREUR_COMPTE_CONNECTEE;
    }

    char * mot_de_passe_chiffre = chiffrer_mot_de_passe(mot_de_passe);

    char *mot_de_passe_base_de_donnees = compte_cherchee->mot_de_passe;

    if (strcmp(mot_de_passe_chiffre, mot_de_passe_base_de_donnees) != 0)
    {
        free(compte_cherchee);
        return ERREUR_MAUVAIS_MOT_DE_PASSE;
    }

    connexion->pseudo = malloc(sizeof(char) * strlen(pseudo) + 1);
    strcpy(connexion->pseudo, pseudo);
    groupe *groupe_cherchee = chercher_groupe_par_id(compte_cherchee->groupe);
    
    if (groupe_cherchee == NULL)
    {
        connexion->groupe = NULL;
    }
    else
    {
        connexion->groupe = malloc(sizeof(char) * strlen(groupe_cherchee->nom) + 1);
        strcpy(connexion->groupe, groupe_cherchee->nom);
    }

    fermer_chiffrement(mot_de_passe_chiffre);
    fermer_groupe(groupe_cherchee);
    fermer_compte(compte_cherchee);
    return OK;
}

int changer_pseudo(connexion_client *connexion, char *nouveau_pseudo, int taille_pseudo)
{
    if (connexion->pseudo == NULL)
    {
        return ERREUR_PAS_CONNECTEE;
    }

    compte *compte_meme_pseudo = chercher_compte_par_pseudo(nouveau_pseudo);

    if (compte_meme_pseudo != NULL)
    {
        return ERREUR_PSEUDO_EXISTANT;
    }

    int resultat_changement_pseudo = modifier_pseudo_compte(connexion->pseudo, nouveau_pseudo);

    if (resultat_changement_pseudo == OK)
    {
        char *pseudo = (char *)malloc(sizeof(char) * (taille_pseudo + 1));
        strcpy(pseudo, nouveau_pseudo);
        free(connexion->pseudo);
        connexion->pseudo = pseudo;
    }

    return resultat_changement_pseudo;
}