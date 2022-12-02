DROP TABLE IF EXISTS historique_messages;
CREATE TABLE historique_messages(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    groupe TEXT NOT NULL,
    emetteur TEXT NOT NULL,
    recepteur TEXT NOT NULL,
    message TEXT NOT NULL
);