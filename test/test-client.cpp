#include "../include/client.hpp"
#include "../include/cli.hpp"
#include <gtest/gtest.h>

class ClientTest : public ::testing::Test {
protected:
    ClientFtp * client;

    void SetUp() override {
        Parameters parameters(
            "ftp.ubuntu.com",
            "toto",
            "azerty",
            0,
            false
        );
        client = new ClientFtp(parameters);
    }

    void TearDown() override {
        delete client;
    }
};


TEST_F(ClientTest, ClientFtpStartsWithCorrectValues) {
    EXPECT_EQ("ftp.ubuntu.com", client->GetAdrrServerFtp());
    EXPECT_EQ("toto", client->GetUsername());
    EXPECT_EQ("azerty", client->GetPassword());
}