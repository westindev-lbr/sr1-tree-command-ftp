## Ce que je sait sur le protocole FTP

Le protocole FTP fait partie de la couche applicative au même titre que HTTP / SSH il est utilisé pour le transfert de fichier entre ordinateurs et est l'un des plus anciens protocoles sur internet.

Il utilise le modèle Client-Server

Conseil :
Étudier la RFC 959 qui définit le protocole FTP.\

Le protocole `FTP` utilise **deux canaux** de communication pour transférer des fichiers :

* le canal de commande.
* le canal de données.

---
> **Le canal de commande:** est utilisé pour envoyer des commandes FTP entre le client et le serveur, telles que la commande de connexion, la commande de changement de répertoire ou la commande de téléchargement de fichier.
---
> **Le canal de données:** est utilisé pour transférer les données elles-mêmes, telles que les fichiers téléchargés ou téléversés.
---

Les commandes `FTP` :

| Commande | Usage           | Description                                              |
|----------|-----------------|----------------------------------------------------------|
| USER     | `USER username` | Envoie le nom d'utilisateur pour l'authentification.     |
| PASS     | `PASS password` | Envoie le mot de passe associé à l'utilisateur.          |
| ACCT     | `ACCT account-info`| Fournit les informations de compte supplémentaires    |
| CWD      | `CWD path`      | Change le répertoire de travail actuel.                  |
| CDUP     | `CDUP`          | Remonte d'un niveau dans l'arborescence des répertoires. |
| QUIT     | `QUIT`          | Termine la session et déconnecte l'utilisateur.          |
| PASV     | `PASV`          | Active le mode passif pour les transferts de données.    |
| RETR     | `RETR filename` | Télécharge un fichier du serveur.                        |
| STOR     | `STOR filename` | Envoie un fichier au serveur.                            |
| LIST     | `LIST path`     | Liste les fichiers et répertoires d'un dossier.          |

### 2. Configurer l'Environnement de Développement

#### Concepts de Base des Sockets TCP

Socket : Une interface entre une application et un réseau.\
Les sockets permettent aux applications de communiquer entre elles, que ce soit sur le même ordinateur ou sur différents ordinateurs connectés via un réseau.

TCP (Transmission Control Protocol) : Un protocole orienté connexion qui garantit la livraison de paquets dans l'ordre et sans erreur.

Port : Un numéro spécifique à travers lequel les communications sont établies et maintenues.

Adresse IP : Identifie l'hôte dans le réseau.

#### Bibliothèques C++ pour les Sockets TCP

Utilisée principalement sur les systèmes UNIX/LINUX.\
Fournit les fonctions :

```cpp
socket(), bind(), listen(), accept(), connect(), send(), recv()
```

, et d'autres pour gérer la communication.

Exemple : <sys/socket.h>, <netinet/in.h>, <arpa/inet.h>.

### Serveur et Client

#### Serveur

* Doit écouter les connexions entrantes, accepter les demandes de connexion et traiter les données reçues des clients.

* Généralement, le serveur est un processus qui s'exécute en continu, en attente de demandes des clients.

* Contient la logique pour initialiser un socket, le lier à une adresse IP et un port, écouter les connexions entrantes et les gérer.

Server.cpp :

```cpp
// Inclure les en-têtes nécessaires
// Initialisation du socket serveur
// Liaison, écoute
// Boucle pour accepter les connexions
// Gérer la communication
// Fermer le socket
```

#### Client

* Initie une demande de connexion au serveur et envoie/reçoit des données.

* Le client est généralement exécuté par l'utilisateur lorsqu'il souhaite interagir avec le serveur.

* Contient la logique pour créer un socket et se connecter à l'adresse IP et au port où le serveur écoute.

Client.cpp :

```cpp
// Inclure les en-têtes nécessaires
// Initialisation du socket client
// Connexion au serveur
// Envoyer/recevoir des données
// Fermer le socket
```
