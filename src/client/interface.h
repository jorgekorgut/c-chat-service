#ifndef INTEFACE_H
#define INTEFACE_H

// Public
void initialiser_interface();

// Private
int demander_entree(char **entree_client, size_t *taille_entree);
void afficher_message_par_default();
void traiter_entree_utilisateur(char *entree_client);
void enlever_saut_ligne(char *message, int taille_message);

#endif