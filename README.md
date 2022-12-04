# Chat
## Sommaire

## Choix architecturalle

## Fonctionalitées implantées

### Authentification
#### Connexion
#### Deconnexion

### Modification
#### Changement de pseudo

### Messagerie
#### Envoie des messages globales
#### Envoie des messages privées
#### Historique des messages
##### Chargement des messages quand l'utilisateur se connecte
##### Chargement de l'historique interface serveur
#### Heure d'envoye des messages

## Detailles fonctionalitées

## Instructions d'initialisation
Il est necessaire d'avoir la librairie sqlite3 afin de pouvoir compiler et executer le programme.
Commandes d'obtention des librairies sur ubuntu 20.04 :
`sudo apt install sqlite3`
`sudo apt install libsqlite3-dev`

Après, pour crier la base de données SQLITE et compiler le programme, vous lancez le script run.
`./run.sh`

Pour finir, vous lancez le serveur et le client. Le serveur est configuré pour écouter sur le port 9999. 
Si vous voulez changer le port, veillez acceder le fichier `src/serveur/serveur.h`
`./serveur_app.o`
`./client_app.o`
