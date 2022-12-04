#ifndef INTERFACE_H
#define INTERFACE_H

// Private
void afficher_ecoute_clients_pret();

void initialiser_interface();

void afficher_fin_application();

int demander_entree(char **entree_client, size_t *taille_entree);

void traiter_entree_serveur(char *entree_client);
void afficher_aide();
void afficher_groupe_aide();

#endif