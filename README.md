# Chat
## Sommaire
 
## Choix architecturaux
![Flux reseaux](/docs/diagrammes_reseaux.drawio.png)

### Architecture reseaux
 
## Fonctionnalitées implantées
 
### Authentification
Connexion
Deconnexion
Chiffrement : Cette fonctionnalité était présente lors du chiffrement des mot de passe dans la base de données.
 
### Modification
Changement de pseudo
 
### Messagerie
Envoie des messages globales
Envoie des messages privées : Possibilité d'envoyer des messages privés à un autre utilisateur.
Historique des messages : Chargement des messages qui sont reliés à l'utilisateur quand il se reconnecte.
Heure d'envoi des messages : Sauvegarde l'information du moment d'envoi d'un message.
 
### Groupes
Création des groupes
Sortir des groupes
Echange de messages au sein d'un groupe : Possibilité d'envoyer des messages privés à un groupe spécifique.

### Persistance
Sauvegarde toutes les informations dans une base de données SQLite, afin de persister les données.
 
## Instructions d'initialisation
Il est nécessaire d'avoir la librairie sqlite3 afin de pouvoir compiler et exécuter le programme.
Commandes d'obtention des librairies sur ubuntu 20.04 :
`sudo apt install sqlite3`
`sudo apt install libsqlite3-dev`
 
Pour le chiffrement la librairie openssl-crypto doit etre instalée.
`sudo apt install libssl-dev`
 
Après, pour créer la base de données SQLITE et compiler le programme, vous lancez le script run.
`./run.sh`
 
Pour finir, vous lancez le serveur et le client. Le serveur est configuré pour écouter sur le port 9999.
Si vous voulez changer le port, veuillez accéder les fichiers `src/serveur/codes_constantes.h` et le fichier `src/client/client.h`
`./serveur_app.o`
`./client_app.o`
 
## Instructions d'utilisation
Afin de voir la totalité des commandes du chat et comprendre comment utiliser l'application. Veuillez rentrer `/aide` sur la console.
 
## Critique du projet
L'application de chat développé a été très riche pour l'apprentissage du fonctionnement du protocole TCP avec des Sockets. En plus du monter en compétence avec l'implémentation détaillé de cette technologie en C, ce projet m'a permis de remarquer l'importance d'une bonne structuration des fichiers et d'une identification totif des tâches. Ce dernier permet l'avancement fluide et correct du développement.
Comme pistes d'amélioration, le projet aurait pu avoir des cryptages des messages au niveau serveur et client, affichages des informations d'un groupe, une interface graphique, ...
De plus, un makefile plus pompeux aurait pu permettre la séparation plus élégante des fichiers.
