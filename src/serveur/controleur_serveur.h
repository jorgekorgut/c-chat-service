#ifndef CONTROLEUR_SERVEUR_H
#define CONTROLEUR_SERVEUR_H

#define MESSAGE_GLOBALE "global"
#define MESSAGE_GROUPE "groupe"
#define MESSAGE_PRIVE "personne"

#define PSEUDO_SERVEUR "serveur"

#include "linkedlist_connexion.h"

//Public
int demander_changement_pseudo(char * pseudo, int taille_pseudo);
int ecrire_a_tous(connexion_client *emetteur, char *message);
int ecrire_a_une_personne(connexion_client *emetteur, char *message, char * pseudo_recepteur);
void initialiser_controleur();
void fermer_controleur();
linkedlist_connexion_client * get_liste_connexions();

#endif