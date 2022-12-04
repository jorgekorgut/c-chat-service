#ifndef CONTROLEUR_SERVEUR_H
#define CONTROLEUR_SERVEUR_H

#define MESSAGE_GLOBALE "globale"
#define MESSAGE_PRIVE "privee"
#define MESSAGE_GROUPE "groupe"

#define ERREUR 0
#define ERREUR_PAS_CONNECTEE -10
#define ERREUR_MAUVAIS_MOT_DE_PASSE -11
#define ERREUR_COMPTE_INEXISTANTE -12
#define ERREUR_SAISIE -20
#define ERREUR_PSEUDO_EXISTANT -21
#define ERREUR_MOT_DE_PASSE_COURT -22
#define OK 1

#define PSEUDO_SERVEUR "serveur"

#include "linkedlist_connexion.h"

//Public
int creer_un_compte(connexion_client *connexion, char *pseudo, char *mot_de_passe);
int connecter_compte(connexion_client *connexion, char *pseudo, char *mot_de_passe);
int demander_changement_pseudo(connexion_client *connexion, char * pseudo, int taille_pseudo);
int envoyer_historique_au_compte(connexion_client *connexion);
int envoyer_message_a_tous(connexion_client *connexion, char *message);
int envoyer_message_privee(connexion_client *connexion, char *message, char * pseudo_recepteur);
int changer_pseudo(connexion_client *connexion, char *nouveau_pseudo, int taille_pseudo);

void initialiser_controleur();
void fermer_controleur();
linkedlist_connexion_client * get_liste_connexions();

//Private
void formater_message(char *pseudo, char *type_message, char *groupe, char *destinataire, char *message, char *message_formatee, time_t date);


#endif