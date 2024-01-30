
#include <netdb.h>
#include <iostream>
#include <string.h>
#include "../include/cli.hpp"
#include "../include/client.hpp"

int main( int argc, char* argv[] ) {

    try {
        CommandLineInterfaceFtpClient cli( argc, argv );
        Parameters params = cli.GetParameters();
        ClientFtp client( params );


        std::cout << "Nom de domaine du serveur FTP : " << client.GetAdrrServerFtp() << std::endl;

        // *** Connexion au serveur FTP -> Ouverture du canal de contrôle
        // *** Etablissement de la connexion FTP
        int connexion_result = client.ConnectToServerProcess();
        std::cout << "Résolution de l'addresse IP du serveur : " << client.GetIp() << std::endl;
        if ( connexion_result == -1 ) {
            std::cerr << "Erreur lors de la connexion au serveur" << std::endl;
            client.CloseControlSocket();
            return -1;
        }
        // ! Réception de la réponse du serveur
        std::pair<int, std::string> response =  client.ReadResponse();
        std::cout << response.first << " " << response.second;

        // ! (1) Envoi de l'utilisateur
        client.SendCommand( Command::USER );
        response =  client.ReadResponse();
        std::cout << response.first << " " << response.second;

        // ! (2) Envoi du mot de passe
        client.SendCommand( Command::PASS );
        response =  client.ReadResponse();
        std::cout << response.first << " " << response.second;

        // ! (3) Envoi de la commande passive
        client.EnterInPassiveMode();

        // *** Connexion à l'hôte pour le canal de données
        int connexion_data_channel = client.ConnectToDataChannelProcess();
        if ( connexion_data_channel == -1 ) {
            std::cerr << "Erreur lors de la création de la socket de données" << std::endl;
            client.CloseDataSocket();
            return -1;
        }

        // ! Envoi de la commande LIST
        client.ListCurrentDirectoryCommand();
        client.ReadResponseDataChannel();


        // client.ChangeDirectory("extras");
        // client.EnterInPassiveMode();
        // client.ConnectToDataChannelProcess();
        // client.ListCurrentDirectoryCommand();
        // client.ReadResponseDataChannel();

        // Affichage de l'arbre
        client.DeepFirstSearch( client.GetTree(), 1 );

        if ( params.GetIsJsonMode() ) {
            client.GetTree()->PrintTreeJson();
        }
        else {
            client.GetTree()->PrintTree();
        }

        std::cout << "\n";

        // ! Envoi de la commande QUIT
        client.SendCommand( Command::QUIT );
        response =  client.ReadResponse();
        std::cout << response.first << " " << response.second << std::endl;

        client.FreeAddrInfoStruct();
        client.CloseDataSocket();
        client.CloseControlSocket();
    }
    catch ( const std::exception& e ) {
        std::cerr << e.what() << '\n';
        return -1;
    }
    return 0;
}
