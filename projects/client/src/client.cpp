#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <string.h>


int main( int argc, char* argv[] ) {

    std::string addr_server_ftp;

    if ( argc < 2 ) {
        std::cerr << "Usage: " << argv[0] << " <adr_server_ftp>" << std::endl;
        return -1;
    }
    else {
        addr_server_ftp = argv[1];
    }

    // Résolution de l'adresse du serveur
    struct addrinfo hints, * res;
    std::memset( &hints, 0, sizeof( hints ) );
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    if ( getaddrinfo( addr_server_ftp.c_str(), "21", &hints, &res ) != 0 ) {
        std::cerr << "Erreur lors de la résolution de l'adresse" << std::endl;
        return -1;
    }

    char host[NI_MAXHOST];
    int err = getnameinfo( res->ai_addr, res->ai_addrlen, host, sizeof( host ), NULL, 0, NI_NUMERICHOST );
    if ( err != 0 ) {
        std::cerr << "Erreur lors de la résolution de l'adresse" << std::endl;
        return -1;
    }

    std::cout << "Adresse du serveur FTP : " << addr_server_ftp << std::endl;
    std::cout << "Résolution de l'addresse du serveur FTP : " << host << std::endl;

    // Création d'un socket
    /**
     * @var int socket_client_id : Identifiant du socket client
     * @var: socket_client_id : un entier qui représente le socket client.
     * @fn: socket() : Création d'un socket et récupération de son identifiant
     * @param: AF_INET : Famille d'adresses du socket (ici IPv4)
     * @param: SOCK_STREAM : Type de socket (ici TCP)
     * @param: 0 : Protocole particulier à utiliser avec le socket (ici aucun choix particulier)
    */
    int socket_client_id = socket( res->ai_family, res->ai_socktype, res->ai_protocol );
    if ( socket_client_id == -1 ) {
        std::cerr << "Erreur lors de la création du socket" << std::endl;
        return -1;
    }


    // Connexion au serveur
    /**
     * @fn: connect(@arg: socket_client_id, @arg: (struct sockaddr *) &addr_server, @arg: sizeof(addr_server))
     *      @arg: socket_client_id : Identifiant du socket client
     *      @arg: (struct sockaddr *) &addr_server : Pointeur vers la structure de l'adresse du serveur
     *      @arg: sizeof(addr_server) : Taille de la structure de l'adresse du serveur
    */
    if ( connect( socket_client_id, res->ai_addr, res->ai_addrlen ) == -1 ) {
        std::cerr << "Erreur lors de la connexion au serveur" << std::endl;
        close( socket_client_id );
        return -1;
    }

    // Etablissement de la connexion FTP
    // WELCOME MESSAGE
    // Réception de la réponse du serveur
    char buffer[1024];
    int size_welcome = recv( socket_client_id, buffer, sizeof( buffer ), 0 );
    if ( size_welcome == -1 ) {
        std::cerr << "Erreur lors de la réception des données" << std::endl;
        return -1;
    }
    std::cout << "Réception de : " << buffer << std::endl;

    // Envoi de l'utilisateur
    int size_user = send( socket_client_id, "USER anonymous\r\n", 16, 0 );
    if ( size_user == -1 ) {
        std::cerr << "Erreur lors de l'envoi des données" << std::endl;
        return -1;
    }

    int size_user_rep = recv( socket_client_id, buffer, sizeof( buffer ), 0 );
    if ( size_user_rep == -1 ) {
        std::cerr << "Erreur lors de la réception des données" << std::endl;
        return -1;
    }
    // Réception de la réponse du serveur
    // 331 Please specify the password.
    std::cout << "Réception de : " << buffer << std::endl;

    // Envoi du mot de passe
    int size_pass = send( socket_client_id, "PASS azerty\r\n", 16, 0 );
    if ( size_pass == -1 ) {
        std::cerr << "Erreur lors de l'envoi des données" << std::endl;
        return -1;
    }

    int size_pass_rep = recv( socket_client_id, buffer, sizeof( buffer ), 0 );
    if ( size_pass_rep == -1 ) {
        std::cerr << "Erreur lors de la réception des données" << std::endl;
        return -1;
    }
    std::cout << "Réception de : " << buffer << std::endl;


    ///////////////// PASSIVE ///////////////////////



      // Envoi de la commande PASV
    int size_pasv = send( socket_client_id, "PASV\r\n", 6, 0 );
    if ( size_pasv == -1 ) {
        std::cerr << "Erreur lors de l'envoi de la commande PASV" << std::endl;
        close( socket_client_id );
        return -1;
    }

    // Réception de la réponse du serveur
    int size_pasv_rep = recv( socket_client_id, buffer, sizeof( buffer ), 0 );
    if ( size_pasv_rep == -1 ) {
        std::cerr << "Erreur lors de la réception de la réponse PASV" << std::endl;
        return -1;
    }

    // Analyse de la réponse pour obtenir l'adresse et le port du canal de données
    // ...
    // Prendre les deux derniers nombres de la réponse au PASV
    // 227 Entering Passive Mode (127,0,0,1,14,1)
    // 14(1er nb) * 256 + 1(2e nb) = 3585
    // 

    // Création d'une nouvelle socket pour le canal de données
    int data_socket_id = socket( AF_INET, SOCK_STREAM, 0 );
    if ( data_socket_id == -1 ) {
        std::cerr << "Erreur lors de la création de la socket de données" << std::endl;
        return -1;
    }

    // Connexion à l'hôte pour le canal de données
    // ...

    // Envoi des données sur le canal de données
    const char* data = "Ceci est un exemple de données";
    int size_data = send( data_socket_id, data, strlen( data ), 0 );
    if ( size_data == -1 ) {
        std::cerr << "Erreur lors de l'envoi des données" << std::endl;
        close( data_socket_id );
        return -1;
    }

    // Fermeture de la socket de données
    close( data_socket_id );


    freeaddrinfo( res ); // N'oubliez pas de libérer la mémoire
    // Fermeture du socket
    close( socket_client_id );
    std::cout << "It's Works !" << std::endl;
    return 0;
}