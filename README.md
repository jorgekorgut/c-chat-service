# Chat
## Sommaire

## Choix architecturalle

### Architecture reseaux

## Fonctionalitées implantées

### Authentification
#### Connexion
#### Deconnexion
#### Encryptation

### Modification
#### Changement de pseudo

### Messagerie
#### Envoie des messages globales
#### Envoie des messages privées
#### Historique des messages
##### Chargement des messages quand l'utilisateur se connecte
#### Heure d'envoye des messages

### Groupes
#### Creation des groupes
#### Echange de messages au sein d'un groupe

## Detailles fonctionalitées

## Instructions d'initialisation
Il est necessaire d'avoir la librairie sqlite3 afin de pouvoir compiler et executer le programme.
Commandes d'obtention des librairies sur ubuntu 20.04 :
`sudo apt install sqlite3`
`sudo apt install libsqlite3-dev`

Pour le chiffrement la librairie openssl-crypto doit etre instalée.
`sudo apt install libssl-dev`

Après, pour crier la base de données SQLITE et compiler le programme, vous lancez le script run.
`./run.sh`

Pour finir, vous lancez le serveur et le client. Le serveur est configuré pour écouter sur le port 9999. 
Si vous voulez changer le port, veillez acceder les fichiers `src/serveur/serveur.h` et le fichier `src/client/client.h`
`./serveur_app.o`
`./client_app.o`
