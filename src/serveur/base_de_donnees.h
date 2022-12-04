#ifndef HISTORIQUE_H
#define HISTORIQUE_H

#include <sqlite3.h>

int initialiser_historique_messages();

int fermer_historique_messages();

sqlite3 * get_base_de_donnees();

void fermer_preparation_requete(sqlite3_stmt *preparation_requete);

#endif