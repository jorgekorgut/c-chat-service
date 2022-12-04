#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include "serveur.h"
#include "controleur_serveur.h"
#include "linkedlist_connexion.h"
#include "communication.h"
#include "base_de_donnees.h"
#include "linkedlist_information_message.h"
#include "codes_type_messages.h"

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