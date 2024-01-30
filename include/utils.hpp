#ifndef UTILS_HPP
#define UTILS_HPP

#include <exception>
#include <string>

class FtpException: public std::exception { 
public:
    FtpException( int code_error, std::string message ) :
        code_error_ { code_error },
        message_ { message } { };
    ~FtpException() throw() { };
    const char* what() const throw() { return message_.c_str(); };
    int GetCodeError() const { return code_error_; };
    std::string GetMessage() const { return message_; };

private:
    int code_error_;
    std::string message_;
};


#endif  //UTILS_HPP