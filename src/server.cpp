#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>


int main() {
    // Création d'un socket
    int socket_server = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_server == -1) {
        std::cerr << "Erreur lors de la création du socket" << std::endl;
        return -1;
    }

    // Création de l'adresse du serveur
    struct sockaddr_in addr_server;
    addr_server.sin_family = AF_INET;
    // Port du serveur
    addr_server.sin_port = htons(8080); 
    // Adresse IP du serveur
    addr_server.sin_addr.s_addr = inet_pton(AF_INET, "127.0.0.1", &addr_server.sin_addr);

    // Liaison du socket
    if (bind(socket_server, (struct sockaddr *) &addr_server, sizeof(addr_server)) == -1) {
        std::cerr << "Erreur lors du bind du socket" << std::endl;
        return -1;
    }

    // Ecoute du socket
    if (listen(socket_server, 10) == -1) {
        std::cerr << "Erreur lors de l'écoute du socket" << std::endl;
        return -1;
    }

    // Buffer de réception
    char buffer[1024];

    // Boucle d'acceptation des connexions
    while (true)
    {
        // Acceptation d'une connexion
        struct sockaddr_in addr_client;
        socklen_t addr_client_size = sizeof(addr_client);

        // Le serveur est bloqué sur l'acceptation d'une connexion
        int socket_client = accept(socket_server, (struct sockaddr *) &addr_client, &addr_client_size);
        if (socket_client == -1) {
            std::cerr << "Erreur lors de l'acceptation de la connexion" << std::endl;
            continue;
        }

        // Affichage de l'adresse du client
        std::cout << "Connexion du client " << inet_ntoa(addr_client.sin_addr) << std::endl;

        // Boucle de communication avec le client
        while (true)
        {
            // Réception des données du client
            int size = recv(socket_client, buffer, sizeof(buffer), 0);
            if (size == -1) {
                std::cerr << "Erreur lors de la réception des données" << std::endl;
                break;
            }

            // Si le client a fermé la connexion
            if (size == 0) {
                std::cout << "Déconnexion du client " << inet_ntoa(addr_client.sin_addr) << std::endl;
                break;
            }

            // Affichage des données reçues
            std::cout << "Données reçues du client " << inet_ntoa(addr_client.sin_addr) << " : " << buffer << std::endl;

            // Envoi d'un message au client
            if (send(socket_client, "Message reçu !", 15, 0) == -1) {
                std::cerr << "Erreur lors de l'envoi du message" << std::endl;
                break;
            }
        }

        // Fermeture du socket client après la communication
        close(socket_client);
    }

    // Fermeture du socket serveur (ne sera jamais atteint sauf si le serveur doit s'arrêter)
    // CTRL + C pour arrêter le serveur
    close(socket_server);

    return 0;
}