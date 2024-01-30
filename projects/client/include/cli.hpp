#ifndef CLI_HPP
#define CLI_HPP

#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

class Parameters {
private:
    std::string addr_server_ftp_;
    std::string username_;
    std::string password_;
    int exploration_depth_;
    bool is_json_mode_;

public:
    Parameters() :
        addr_server_ftp_ { "" },
        username_ { "" },
        password_ { "" },
        exploration_depth_ { 1 },
        is_json_mode_ { false } { };
    Parameters(
        std::string addr_server_ftp,
        std::string username,
        std::string password,
        int exploration_depth,
        bool is_json_mode
    ) :
        addr_server_ftp_ { addr_server_ftp },
        username_ { username },
        password_ { password },
        exploration_depth_ { exploration_depth },
        is_json_mode_ { is_json_mode } { };
    
    std::string GetAdrrServerFtp() const;
    std::string GetUsername() const;
    std::string GetPassword() const;
    int GetExplorationDepth() const;
    bool GetIsJsonMode() const;
    void SetExplorationDepth( int exploration_depth );
    void SetServerAddress( std::string addr_server_ftp );
    void SetUsername( std::string username );
    void SetPassword( std::string password );
    void SetIsJsonMode( bool is_json_mode );
};


class CommandLineInterfaceFtpClient {
private:
    Parameters parameters_;
public:
    CommandLineInterfaceFtpClient( int argc, char* argv[] );
    void ParseOptions( int argc, char* argv[] );
    void ParsePosArguments( int argc, char* argv[] );
    void ShowHelp( std::string program_name );


    std::vector<std::string> GetArgs() const;
    Parameters GetParameters() const;
};

#endif  //CLI_HPP