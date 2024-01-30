#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../include/cli.hpp"
#include "../include/tree.hpp"
#include <string>
#include <netdb.h>

enum class Command {
    USER,
    PASS,
    SYST,
    PWD,
    CWD,
    CDUP,
    QUIT,
    PASV,
    PORT,
    TYPE,
    RETR,
    STOR,
    LIST,
    NLST,
    HELP,
    NOOP,
    UNKNOWN
};


class ClientFtp {
public:
    // Constructeur
    ClientFtp( Parameters parameters );

    // Getters
    std::string GetAdrrServerFtp() const;
    std::string GetUsername() const;
    std::string GetPassword() const;
    std::string GetIp() const;
    int GetControlSocketId() const;

    // Méthodes Connexions
    int ConnectToServerProcess();
    int ConnectToDataChannelProcess();

    // Méthodes d'envoi et de réception de données
    void SendCommand( enum Command command );
    void ChangeDirectory( std::string arg_cmd );
    void BackToParentDirectory();
    void EnterInPassiveMode();
    void ListCurrentDirectoryCommand();
    void ReadResponseDataChannel();
    std::pair<int, std::string> ReadResponse();


    // Méthodes de nettoyage
    void FlushDataBuffer();
    void FreeAddrInfoStruct();
    void CloseControlSocket();
    void CloseDataSocket();

    // Méthodes de gestion de l'arbre
    void InitTree();
    void DeepFirstSearch( Tree* tree, int depth );
    Tree* GetTree() const;

private:
    std::string addr_server_ftp_;       // Adresse du serveur FTP
    std::string username_;              // Nom d'utilisateur
    std::string password_;              // Mot de passe
    int exploration_depth_;             // Profondeur d'exploration
    char ip_[INET_ADDRSTRLEN];          // Adresse IP du serveur FTP
    char buffer_data_[4096];            // Buffer de réception des données du canal de données
    int control_socket_id_;             // Identifiant du socket
    int data_socket_id_;                // Identifiant du socket du canal de données
    int data_port_;                     // Port du canal de données
    struct addrinfo hints_;             // struct addrinfo hints_ : Structure d'informations sur le socket
    struct addrinfo* result_;           // Pointeur vers la structure d'informations sur le socket
    struct sockaddr_in data_address_;   // Structure d'informations sur le socket du canal de données
    Tree* tree_ = new Tree();           // Arbre de fichiers et répertoires
    std::string arg_cmd_;               // Argument de la commande  

    void SetHints();
    void AddrInfoResolver();
    void SetControlSocketId();
    void ComputeDataPort( std::string response );
    void SetDataAddress();
    void SetSocketDataId();
    std::string ParseCommand( enum Command cmd ) const;
};

#endif  //CLIENT_HPP