DROP TABLE IF EXISTS comptes;
CREATE TABLE comptes(
    id_compte INTEGER PRIMARY KEY AUTOINCREMENT,
    pseudo TEXT NOT NULL,
    mot_de_passe TEXT NOT NULL,
    enline BOOLEAN NOT NULL,
    id_groupe INTEGER
);

DROP TABLE IF EXISTS groupes;
CREATE TABLE groupes(
    id_groupe INTEGER PRIMARY KEY AUTOINCREMENT,
    nom TEXT NOT NULL
);

DROP TABLE IF EXISTS historique_messages;
CREATE TABLE historique_messages(
    id_message INTEGER PRIMARY KEY AUTOINCREMENT,
    id_groupe INTEGER,
    id_emetteur INTEGER NOT NULL,
    id_recepteur INTEGER,
    message TEXT NOT NULL,
    date_envoye INTEGER NOT NULL,
    type_message TEXT NOT NULL,

    CONSTRAINT clef_etrangere_emetteur
        FOREIGN KEY (id_emetteur)
        REFERENCES comptes (id)

    CONSTRAINT clef_etrangere_recepteur
        FOREIGN KEY (id_recepteur)
        REFERENCES comptes (id)
    
    CONSTRAINT clef_etrangere_groupe
        FOREIGN KEY (id_groupe)
        REFERENCES groupes (id)
);