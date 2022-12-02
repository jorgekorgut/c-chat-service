#include <stdio.h>
#include <string.h>
#include "interface.h"
#include "serveur.h"

void initialiser_interface()
{
    afficher_ecoute_clients_pret();
    int sortir = 0;
    while(sortir == 0){
        int tailleEntree= 0;
        char entree[50];
        scanf("%50s%n",entree,&tailleEntree);
        if(strcmp(entree,"sortir") == 0 || strcmp(entree,"s") == 0)
        {
            sortir = 1;
            afficher_fin_application();
        }
    }
}

void afficher_ecoute_clients_pret()
{
    printf("Serveur bien inicialise. En attente de connexion sur le port %d\n", PORT);
}

void afficher_fin_application()
{
    printf("Le serveur s'etteint.\n");
}