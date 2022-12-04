// Sources : https://www.openssl.org/docs/man3.0/man7/crypto.html
#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#include "chiffrement.h"

char *chiffrer_mot_de_passe(char *mot_de_passe)
{
    EVP_MD_CTX *contexte = NULL;
    EVP_MD *sha256 = NULL;

    unsigned int len = 0;
    unsigned char *mot_de_passe_chiffre = NULL;

    /* Create a context for the digest operation */
    contexte = EVP_MD_CTX_new();
    if (contexte == NULL)
    {
        perror("Erreur dans la creation d'un contexte.\n");
        exit(1);
    }

    /*
     * Fetch the SHA256 algorithm implementation for doing the digest. We're
     * using the "default" library context here (first NULL parameter), and
     * we're not supplying any particular search criteria for our SHA256
     * implementation (second NULL parameter). Any SHA256 implementation will
     * do.
     */
    sha256 = EVP_MD_fetch(NULL, "SHA256", NULL);
    if (sha256 == NULL)
    {
        perror("Erreur dans la recuperation de l'algorithme sha256.\n");
        exit(1);
    }

    /* Initialise the digest operation */
    if (!EVP_DigestInit_ex(contexte, sha256, NULL))
    {
        perror("Erreur dans l'initialisation de l'operation de digestion.\n");
        exit(1);
    }

    /*
     * Pass the message to be digested. This can be passed in over multiple
     * EVP_DigestUpdate calls if necessary
     */
    if (!EVP_DigestUpdate(contexte, mot_de_passe, sizeof(char) * strlen(mot_de_passe) + 1))
    {
        perror("Erreur dans l'operation de digestion.\n");
        exit(1);
    }

    /* Allocate the output buffer */
    mot_de_passe_chiffre = OPENSSL_malloc(EVP_MD_get_size(sha256));
    if (mot_de_passe_chiffre == NULL)
    {
        perror("Erreur dans l'allocation du buffer de sortie.\n");
        exit(1);
    }

    /* Now calculate the digest itself */
    if (!EVP_DigestFinal_ex(contexte, mot_de_passe_chiffre, &len))
    {
        perror("Erreur dans le calcul de digestion.\n");
        exit(1);
    }

    EVP_MD_free(sha256);
    EVP_MD_CTX_free(contexte);

    mot_de_passe_chiffre[len - 1] = '\0';
    return (char *)mot_de_passe_chiffre;
}

void fermer_chiffrement(char *valeur_chiffree)
{
    OPENSSL_free(valeur_chiffree);
}