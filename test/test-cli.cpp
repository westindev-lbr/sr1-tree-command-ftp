#include <iostream>
#include <string>
#include <unistd.h>
#include <gtest/gtest.h>
#include "../include/cli.hpp"

class CommandLineInterfaceFtpClientTestWithNoArg : public ::testing::Test {
protected:
    CommandLineInterfaceFtpClient* cli;

    void SetUp() override {
        static char* test_args[] = { "program_name" };
        static int num_args = sizeof( test_args ) / sizeof( char* );

        std::cout << "num_args: " << num_args << std::endl;
        std::cout << "test_args[0]: " << test_args[0] << std::endl;
        cli = new CommandLineInterfaceFtpClient( num_args, test_args );
    }

    void TearDown() override {
        delete cli;
    }
};

class CommandLineInterfaceFtpClientTestWithOneArg : public ::testing::Test {
protected:
    CommandLineInterfaceFtpClient* cli;

    void SetUp() override {
        static char* test_args[] = { "program_name", "ftp.ubuntu.com" };
        static int num_args = sizeof( test_args ) / sizeof( char* );

        cli = new CommandLineInterfaceFtpClient( num_args, test_args );
    }

    void TearDown() override {
        delete cli;
    }
};

class CommandLineInterfaceFtpClientTestWithTwoArg : public ::testing::Test {
protected:
    CommandLineInterfaceFtpClient* cli;

    void SetUp() override {
        static char* test_args[] = { "program_name", "ftp.ubuntu.com", "toto" };
        static int num_args = sizeof( test_args ) / sizeof( char* );

        cli = new CommandLineInterfaceFtpClient( num_args, test_args );
    }

    void TearDown() override {
        delete cli;
    }
};

class CommandLineInterfaceFtpClientTestWithThreeArg : public ::testing::Test {
protected:
    CommandLineInterfaceFtpClient* cli;

    void SetUp() override {
        static char* test_args[] = { "program_name", "ftp.ubuntu.com", "toto", "azerty" };
        static int num_args = sizeof( test_args ) / sizeof( char* );

        cli = new CommandLineInterfaceFtpClient( num_args, test_args );
    }

    void TearDown() override {
        delete cli;
    }
};

class CommandLineInterfaceFtpClientTestWithOnePosArgOneOption : public ::testing::Test {
protected:
    CommandLineInterfaceFtpClient* cli;

    void SetUp() override {
        static char* test_args[] = { "program_name", "-p", "3", "ftp.ubuntu.com" };
        static int num_args = sizeof( test_args ) / sizeof( char* );

        cli = new CommandLineInterfaceFtpClient( num_args, test_args );
    }

    void TearDown() override {
        delete cli;
    }
};

TEST( CommandLineInterfaceFtpClientTest, ThrowsExceptionWhenNoArgumentsAreProvided ) {
    char* argv[] = { "program_name" };
    int argc = sizeof(argv) / sizeof(char*);
    EXPECT_THROW( CommandLineInterfaceFtpClient cli(argc, argv);, std::invalid_argument );
}

TEST( CommandLineInterfaceFtpClientTest, ShouldReturnServerAddress ) {
    char* argv[] = { "program_name", "ftp.ubuntu.com" };
    int argc = sizeof(argv) / sizeof(char*);
    CommandLineInterfaceFtpClient cli(argc, argv);
    EXPECT_EQ( "ftp.ubuntu.com", cli.GetParameters().GetAdrrServerFtp() );
    EXPECT_EQ( "anonymous", cli.GetParameters().GetUsername() );
    EXPECT_EQ( "", cli.GetParameters().GetPassword() );
}

TEST( CommandLineInterfaceFtpClientTest, ShouldReturnServerAddressAndUsername ) {
    char* argv[] = { "program_name", "ftp.ubuntu.com", "toto" };
    int argc = sizeof(argv) / sizeof(char*);
    CommandLineInterfaceFtpClient cli(argc, argv);
    EXPECT_EQ( "ftp.ubuntu.com", cli.GetParameters().GetAdrrServerFtp() );
    EXPECT_EQ( "toto", cli.GetParameters().GetUsername() );
    EXPECT_EQ( "", cli.GetParameters().GetPassword() );
}

TEST_F( CommandLineInterfaceFtpClientTestWithOneArg, ShouldResturnFtpServerAddressPassedAsArg ) {
    Parameters params = cli->GetParameters();
    EXPECT_EQ( "ftp.ubuntu.com", params.GetAdrrServerFtp() );
}

TEST_F( CommandLineInterfaceFtpClientTestWithTwoArg, ShouldReturnAllArgsPassed ) {
    Parameters params = cli->GetParameters();
    EXPECT_EQ( "ftp.ubuntu.com", params.GetAdrrServerFtp() );
    EXPECT_EQ( "toto", params.GetUsername() );
    EXPECT_EQ( "", params.GetPassword() );
}

TEST_F( CommandLineInterfaceFtpClientTestWithThreeArg, ShouldReturnAllArgsPassed ) {
    Parameters params = cli->GetParameters();
    EXPECT_EQ( "ftp.ubuntu.com", params.GetAdrrServerFtp() );
    EXPECT_EQ( "toto", params.GetUsername() );
    EXPECT_EQ( "azerty", params.GetPassword() );
}

TEST_F( CommandLineInterfaceFtpClientTestWithOnePosArgOneOption, ShouldReturnAllArgsPassed ) {
    Parameters params = cli->GetParameters();
    EXPECT_EQ( "ftp.ubuntu.com", params.GetAdrrServerFtp() );
    EXPECT_EQ( 3, params.GetExplorationDepth() );
}
