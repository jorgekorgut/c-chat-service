#ifndef LINKEDLIST_CONNEXION_H
#define LINKEDLIST_CONNEXION_H

//Public

typedef struct connexion_client_t {
    struct sockaddr_in * adresse;
    pthread_t * thread_client;
    int descripteur_socket_client;
    char * pseudo;

    struct connexion_client_t * prochain;
    struct connexion_client_t * ancient;
} connexion_client;

typedef struct linkedlist_connexion_client_t{
    connexion_client * debut;
    int taille;
} linkedlist_connexion_client;

int initialiser_linkedlist_connexions(linkedlist_connexion_client * liste);

int ajouter_connexion_client(linkedlist_connexion_client * liste, connexion_client * connexion);

connexion_client *trouver_connexion_par_description_socket(linkedlist_connexion_client *liste, int description_socket);

connexion_client *trouver_connexion_par_pseudo(linkedlist_connexion_client *liste, char * pseudo);

int enlever_connexion(linkedlist_connexion_client *liste, connexion_client *enlever);

int enlever_connexion_par_description_socket(linkedlist_connexion_client *liste, int descripteur_socket);

int fermer_linkedlist_connexions();

int fermer_connexion(connexion_client *eliminer);

#endif