#include "../include/client.hpp"
#include "../include/cli.hpp"
#include "../include/utils.hpp"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sstream>
#include <cstring>

ClientFtp::ClientFtp( Parameters parameters ) :
    addr_server_ftp_ { parameters.GetAdrrServerFtp() },
    username_ { parameters.GetUsername() },
    password_ { parameters.GetPassword() },
    exploration_depth_ { parameters.GetExplorationDepth() } { };


std::string ClientFtp::GetAdrrServerFtp() const {
    return this->addr_server_ftp_;
}

std::string ClientFtp::GetUsername() const {
    return this->username_;
}

std::string ClientFtp::GetPassword() const {
    return this->password_;
}

std::string ClientFtp::GetIp() const {
    return this->ip_;
}

int ClientFtp::GetControlSocketId() const {
    return this->control_socket_id_;
}

void ClientFtp::SetHints() {
    memset( &hints_, 0, sizeof( struct addrinfo ) );
    hints_.ai_family = AF_INET;       // IPv4
    hints_.ai_socktype = SOCK_STREAM; // TCP
}

// Remplissage de la structure res avec les informations sur le socket
void ClientFtp::AddrInfoResolver() {
    if ( getaddrinfo( addr_server_ftp_.c_str(), "21", &hints_, &result_ ) != 0 ) {
        std::cerr << "Erreur lors de la résolution de l'adresse" << std::endl;
        return;
    }

    /**
     * @fn: getnameinfo(
     *      @a res->ai_addr,     -> Pointeur vers la structure de l'adresse du serveur
     *      @a res->ai_addrlen,  -> Taille de la structure de l'adresse du serveur
     *      @a host,             -> Pointeur vers la chaine de caractères qui va contenir l'adresse du serveur
     *      @a sizeof( host ),   -> Taille de la chaine de caractères qui va contenir l'adresse du serveur
     *      @a NULL,             -> Pointeur vers la chaine de caractères qui va contenir le nom du serveur
     *      @a 0,                -> Taille de la chaine de caractères qui va contenir le nom du serveur
     *      @a NI_NUMERICHOST    -> Option pour récupérer l'adresse du serveur en notation numérique
    */
    int err = getnameinfo( result_->ai_addr, result_->ai_addrlen, ip_, sizeof( ip_ ), NULL, 0, NI_NUMERICHOST );
    if ( err != 0 ) {
        std::cerr << "Erreur lors de la résolution de l'adresse" << std::endl;
        return;
    }
}


void ClientFtp::SetControlSocketId() {
    /**
     * @func: socket()  : Création d'un socket et récupération de son identifiant
     * @a res->ai_family   : Famille d'adresses du socket (ici IPv4)
     * @a res->ai_socktype : Type de socket (ici TCP)
     * @a res->ai_protocol : Protocole particulier à utiliser avec le socket (ici 0 aucun choix particulier)
     */
    this->control_socket_id_ = socket( result_->ai_family, result_->ai_socktype, result_->ai_protocol );
    if ( this->control_socket_id_ == -1 ) {
        std::cerr << "Erreur lors de la création du socket" << std::endl;
        return;
    }
}

int ClientFtp::ConnectToServerProcess() {
    // *** Résolution de l'adresse du serveur
    // *** Obtention de l'adresse IP du serveur 
    SetHints();
    AddrInfoResolver();
    // Configuration du socket de contrôle
    SetControlSocketId();
    /**
     * @func: connect(
     *      @a socket_client_id : Identifiant du socket client
     *      @a res->ai_addr     : Pointeur vers la structure de l'adresse du serveur
     *      @a res->ai_addrlen  : Taille de la structure de l'adresse du serveur
    */
    return connect( this->control_socket_id_, result_->ai_addr, result_->ai_addrlen );
}

void ClientFtp::CloseControlSocket() {
    close( this->control_socket_id_ );
}

void ClientFtp::FreeAddrInfoStruct() {
    freeaddrinfo( result_ );
}

void ClientFtp::FlushDataBuffer() {
    memset( buffer_data_, 0, sizeof( buffer_data_ ) );
}

/**
 * @func: send( -> Envoyer des données sur un socket
 *      @a socket_client_id : Identifiant du socket client
 *      @a COMMAND : Commande à envoyer
 *      @a sizeof( COMMAND ) : Taille de la commande à envoyer
 *      @a 0 : Flags
 */
void ClientFtp::SendCommand( enum Command command ) {
    std::string cmd = ParseCommand( command );
    ssize_t size_send  = send( this->control_socket_id_, cmd.c_str(), strlen( cmd.c_str() ), 0 );
    if ( size_send == -1 ) {
        std::cerr << "Erreur lors de l'envoi des données" << std::endl;
        return;
    }
}



std::string ClientFtp::ParseCommand( enum Command cmd ) const {
    switch ( cmd ) {
    case Command::USER:
        return "USER " + username_ + "\r\n";
    case Command::PASS:
        return "PASS " + password_ + "\r\n";
    case Command::PASV:
        return "PASV\r\n";
    case Command::LIST:
        return "LIST\r\n";
    case Command::CWD:
        return "CWD " + arg_cmd_ + "\r\n";
    case Command::CDUP:
        return "CDUP\r\n";
    case Command::QUIT:
        return "QUIT\r\n";
    default:
        return "UNKNOWN";
    }
}

void ClientFtp::ComputeDataPort( const std::string response ) {
    // Analyse de la réponse pour obtenir l'adresse et le port du canal de données
    // ...
    // Prendre les deux derniers nombres de la réponse au PASV
    // 227 Entering Passive Mode (127,0,0,1,14,1)
    // 14(1er nb) * 256 + 1(2e nb) = 3585


    // Trouver les index des deux dernières virgules
    size_t index_of_last_comma = response.find_last_of( ',' );
    size_t index_of_penultimate_comma = response.find_last_of( ',', index_of_last_comma - 1 );

    // Extraire les deux derniers nombres de la réponse
    std::string penultimate_number_str = response.substr( index_of_penultimate_comma + 1, index_of_last_comma - index_of_penultimate_comma - 1 );
    std::string last_number_str = response.substr( index_of_last_comma + 1 );

    // Conversion des nombres en entiers
    int penultimate_number = std::stoi( penultimate_number_str );
    int last_number = std::stoi( last_number_str );

    // Calcul du port
    this->data_port_ = penultimate_number * 256 + last_number;
}

void ClientFtp::EnterInPassiveMode() {
    // *** Envoi de la commande passive
    SendCommand( Command::PASV );
    // *** Réception de la réponse du serveur
    std::pair<int, std::string> res = ReadResponse();
    // std::cout << res.first << " " << res.second << std::endl;
    if ( res.first != 227 ) {
        std::cerr << "Erreur lors de l'entrée en mode passif" << std::endl;
        throw FtpException { res.first, res.second };
    }
    ComputeDataPort( res.second );
}


void ClientFtp::SetDataAddress() {
    // *** Configuration de l'adresse pour le canal de données
    std::memset( &this->data_address_, 0, sizeof( this->data_address_ ) );
    this->data_address_.sin_family = AF_INET;
    this->data_address_.sin_port = htons( this->data_port_ );
    inet_pton( AF_INET, this->ip_, &( this->data_address_.sin_addr ) );
}


// Ouverture du canal de données
void ClientFtp::SetSocketDataId() {
    this->data_socket_id_ = socket( AF_INET, SOCK_STREAM, 0 );
    if ( this->data_socket_id_ == -1 ) {
        std::cerr << "Erreur lors de la création du socket" << std::endl;
        return;
    }
}

// Connexion au serveur FTP
int ClientFtp::ConnectToDataChannelProcess() {
    // *** Configuration de l'adresse pour le canal de données
    SetDataAddress();
    // *** Création d'une nouvelle socket pour le canal de données
    SetSocketDataId();
    return connect( this->data_socket_id_, ( struct sockaddr* ) &data_address_, sizeof( data_address_ ) );
}

void ClientFtp::CloseDataSocket() {
    close( this->data_socket_id_ );
}

void ClientFtp::ReadResponseDataChannel() {
    FlushDataBuffer();
    ssize_t size_rec  = recv( this->data_socket_id_, buffer_data_, sizeof( buffer_data_ ), 0 );
    if ( size_rec == -1 ) {
        std::cerr << "Erreur lors de la réception des données" << std::endl;
        throw FtpException { -1, "Erreur de lecture du socket" };
    }
    buffer_data_[size_rec] = '\0';
    // std::cout << "\nDonnées reçues : \n" << buffer_data_ << std::endl;
    std::pair<int, std::string> res =  this->ReadResponse();
    // std::cout << res.first << " " << res.second;
    if ( res.first != 226 ) {
        std::cerr << "Erreur lors de la réception des données" << std::endl;
        throw FtpException { res.first, res.second };
    }
}

void ClientFtp::InitTree() {
    this->tree_ = new Tree();
}

void ClientFtp::DeepFirstSearch( Tree* current_tree, int depth ) {
    // Si la profondeur maximale est atteinte, retourner
    if ( depth > exploration_depth_ ) {
        return;
    }
    if ( current_tree->GetIsDir() ) {

        // Changement de répertoire
        ChangeDirectory( current_tree->GetName() );

        EnterInPassiveMode();
        ConnectToDataChannelProcess();
        ListCurrentDirectoryCommand();
        ReadResponseDataChannel();
        current_tree->InitTree( buffer_data_ );
        // Parcours des enfants
        for ( auto child : current_tree->GetChildren() ) {

            try {
                DeepFirstSearch( child, depth + 1 );
            }
            catch ( FtpException& e ) {
                if ( e.GetCodeError() == 550 ) {
                    std::cerr << "EXCEPTIONS => " << e.what() << '\n';
                    // Passe à l'élément suivant
                    continue;
                }
                else {
                    // Fermeture des sockets et libération de la mémoire
                    // Avant tentative de reconnexion
                    CloseControlSocket();
                    CloseDataSocket();

                    // Reconnexion au serveur
                    ConnectToServerProcess();
                    std::pair<int, std::string> response = ReadResponse();
                    std::cout << response.first << " " << response.second;

                    // Login
                    SendCommand( Command::USER );
                    response =  ReadResponse();
                    std::cout << response.first << " " << response.second;

                    SendCommand( Command::PASS );
                    response =  ReadResponse();
                    std::cout << response.first << " " << response.second;

                    // Changement de répertoire
                    EnterInPassiveMode();
                    ConnectToDataChannelProcess();
                    ListCurrentDirectoryCommand();
                    ReadResponseDataChannel();
                    current_tree->InitTree( buffer_data_ );
                }

            }

        }

        // Retour au répertoire parent
        BackToParentDirectory();
    }
}


void ClientFtp::ChangeDirectory( std::string dir_name ) {
    this->arg_cmd_ = dir_name;
    SendCommand( Command::CWD );
    std::pair<int, std::string> res = ReadResponse();
    // std::cout << res.first << " " << res.second << std::endl;
    if ( res.first != 250 ) {
        std::cerr << "Erreur lors du changement de répertoire" << std::endl;
        throw FtpException { res.first, res.second };
    }
}

void ClientFtp::BackToParentDirectory() {
    SendCommand( Command::CDUP );
    std::pair<int, std::string> res = ReadResponse();
    if ( res.first != 250 ) {
        std::cerr << "Erreur lors du changement de répertoire" << std::endl;
        throw FtpException { res.first, res.second };
    }
}

Tree* ClientFtp::GetTree() const {
    return this->tree_;
}


std::pair<int, std::string> ClientFtp::ReadResponse() {
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    std::string fullResponse;

    /**
     * @func: recv( -> Lire des données sur un socket
     *      @a socket_client_id : Identifiant du socket client
     *      @a buffer_ : Pointeur vers le buffer_ de réception des données
     *      @a sizeof( buffer_ ) : Taille du buffer_ de réception des données
     */
    ssize_t bytesReceived = recv( control_socket_id_, buffer, BUFFER_SIZE - 1, 0 );
    if ( bytesReceived < 1 ) {
        // Gérer l'erreur de lecture
        throw FtpException { -1, "Erreur de lecture du socket" };
    }

    buffer[bytesReceived] = '\0';
    fullResponse = buffer;

    // Analyser le code de réponse
    int statusCode = std::stoi( fullResponse.substr( 0, 3 ) );
    std::string message = fullResponse.substr( 4 );
    if ( statusCode > 400 ) {
        throw FtpException { statusCode, message };
    }
    return { statusCode, message };
}

void ClientFtp::ListCurrentDirectoryCommand() {
    SendCommand( Command::LIST );
    std::pair<int, std::string> res = ReadResponse();
    // std::cout << res.first << " " << res.second << std::endl;
    if ( res.first != 150 ) {
        std::cerr << "Erreur lors de la réception des données" << std::endl;
        throw FtpException { res.first, res.second };
    }
}