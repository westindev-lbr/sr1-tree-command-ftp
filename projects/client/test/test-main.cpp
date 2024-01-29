#include <gtest/gtest.h>

int main( int argc, char** argv ) {
    ::testing::InitGoogleTest( &argc, argv );
    // Ajoutez ces lignes pour activer l'affichage verbeux
    ::testing::GTEST_FLAG( color ) = "yes";
    ::testing::GTEST_FLAG( filter ) = "CommandLineInterfaceFtpClientTest*.*";
    return RUN_ALL_TESTS();
    return 0;
}