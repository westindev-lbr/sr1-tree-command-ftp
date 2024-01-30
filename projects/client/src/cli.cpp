#include "../include/cli.hpp"

//////////////////////////////////////////////////
// CommandLineInterfaceFtpClient Implementation //
//////////////////////////////////////////////////

CommandLineInterfaceFtpClient::CommandLineInterfaceFtpClient( int argc, char* argv[] ) {

    if ( argc < 2 ) {
        throw std::invalid_argument( "Arguments insuffisants : utilisez l'option -h pour consulter l'aide" );
    }
    parameters_ = Parameters();
    ParseOptions( argc, argv );
    ParsePosArguments( argc, argv );
}


void CommandLineInterfaceFtpClient::ParseOptions( int argc, char* argv[] ) {
    int opt;
    while ( ( opt = getopt( argc, argv, "hL:J" ) ) != -1 ) {
        switch ( opt ) {
        case 'h':
            ShowHelp( argv[0] );
            break;
        case 'L':
            parameters_.SetExplorationDepth( std::stoi( optarg ) );
            break;
        case 'J':
            parameters_.SetIsJsonMode( true );
            break;
        case '?':
            // Traitement des erreurs d'option
            break;
        default:
            throw std::invalid_argument( "Argument non reconnu. Utilisez -h pour l'aide." );
        }

    }
}

void CommandLineInterfaceFtpClient::ParsePosArguments( int argc, char* argv[] ) {
    if ( optind < argc ) {
        std::string addr_server_ftp = argv[optind++];
        std::string username = optind < argc ? argv[optind++] : "anonymous";
        std::string password = optind < argc ? argv[optind++] : "";
        parameters_.SetServerAddress( addr_server_ftp );
        parameters_.SetUsername( username );
        parameters_.SetPassword( password );
    }
    else {
        throw std::invalid_argument( "Usage: " + std::string( argv[0] ) + " [ftp_server_address] or -h for consult help" );
    }
}

void CommandLineInterfaceFtpClient::ShowHelp( std::string program_name ) {
    std::cout << "Utilisation: " << program_name << " [options] [ftp_server_address] [username] [password]" << std::endl;
    std::cout << "\n\033[1mOPTIONS\033[0m" << std::endl;
    std::cout <<    "\t-h \t\t\taffiche l'aide" << std::endl;
    std::cout <<    "\t-L \033[4mlevel\033[0m \t\tprofondeur d'exploration" << std::endl;
    std::cout << "\n\033[1mARGUMENTS\033[0m" << std::endl;
    std::cout <<    "\tftp_server_address \tadresse du serveur FTP" << std::endl;
    std::cout <<    "\tusername \t\tnom d'utilisateur" << std::endl;
    std::cout <<    "\tpassword \t\tmot de passe" << std::endl;

    // Fin du programme
    exit( 0 );
}

Parameters CommandLineInterfaceFtpClient::GetParameters() const {
    return parameters_;
}

//////////////////////////////////////////////
// Parameters Implementation                //  
//////////////////////////////////////////////

void Parameters::SetExplorationDepth( int exploration_depth ) {
    exploration_depth_ = exploration_depth;
}
void Parameters::SetServerAddress( std::string addr_server_ftp ) {
    addr_server_ftp_ = addr_server_ftp;
}
void Parameters::SetUsername( std::string username ) {
    username_ = username;
}
void Parameters::SetPassword( std::string password ) {
    password_ = password;
}

void Parameters::SetIsJsonMode( bool is_json_mode ) {
    is_json_mode_ = is_json_mode;
}

std::string Parameters::GetAdrrServerFtp() const {
    return addr_server_ftp_;
}

std::string Parameters::GetUsername() const {
    return username_;
}

std::string Parameters::GetPassword() const {
    return password_;
}

int Parameters::GetExplorationDepth() const {
    return exploration_depth_;
}

bool Parameters::GetIsJsonMode() const {
    return is_json_mode_;
}