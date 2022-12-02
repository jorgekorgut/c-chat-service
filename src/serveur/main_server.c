#include "serveur.h"
#include "controleur_serveur.h"

int main(int argc, char **argv){
    initialiser_controleur();
    initialiser_serveur();
    fermer_serveur();
    fermer_controleur();
    return 0;
}