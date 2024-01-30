#include <gtest/gtest.h>
#include "../include/tree.hpp"
#include "../include/client.hpp"




class TreeTest : public ::testing::Test {
protected:
    ClientFtp* client;
    Tree* tree;

    void SetUp() override {
        tree = new Tree();
        client = new ClientFtp( Parameters() );
    }

    void TearDown() override {
        delete tree;
    }

    void AddChild( Tree* child ) {
        tree->AddChild( child );
    }

};


TEST_F( TreeTest, TreeStartsWithCorrectValues ) {
    tree->InitTree( "drwxr-xr-x  11 ftp      ftp          4096 Aug 31 14:27 pub" );
    EXPECT_EQ( ".", tree->GetName() );
    EXPECT_EQ( true, tree->GetIsDir() );
    EXPECT_EQ( nullptr, tree->GetParent() );
    EXPECT_EQ( 1, tree->GetChildren().size() );
}

TEST_F( TreeTest, TreeAddChild ) {
    Tree* child = new Tree();
    AddChild( child );
    EXPECT_EQ( 1, tree->GetChildren().size() );
}

TEST_F( TreeTest, EmptyTreeConvertJson ) {
    std::string json = R"([
  {"type":"directory","name":"."}
])";
    EXPECT_EQ( json, tree->ConvertTreeToJson() );
}

TEST_F( TreeTest, TreeRootConvertJson ) {
    std::string buffer_data = "-rwxr-xr-x  11 ftp      ftp          4096 Aug 31 14:27 pub";
    tree->InitTree( buffer_data.c_str() );

    std::string json_string = tree->ConvertTreeToJson();

    std::string json_expected = R"([
  {"type":"directory","name":".","contents":[
    {"type":"file","name":"pub"}
  ]}])";

    EXPECT_EQ( json_expected, json_string );
}


TEST_F(TreeTest, NbDirAndFiles) {
    std::string buffer_data = "-rwxr-xr-x  11 ftp      ftp          4096 Aug 31 14:27 pub";
    tree->InitTree( buffer_data.c_str() );
    std::pair<int,int> nb_dir_file =  tree->UpdateCountDirFileRecursive();
    EXPECT_EQ( 1, nb_dir_file.first );
    EXPECT_EQ( 1, nb_dir_file.second );
}


// TEST_F( TreeTest, TreeWith10ChildsConvertJson ) {

//     std::string buffer_data = "-rw-r--r--  1 axellbr  staff    4639 29 jan 16:23 test-cli.cpp \n";
//         buffer_data += "-rw-r--r--  1 axellbr  staff    4639 29 jan 16:23 test-cli.cpp\n";
//         buffer_data += "-rw-r--r--  1 axellbr  staff  119584 29 jan 16:23 test-cli.o\n";
//         buffer_data += "-rw-r--r--  1 axellbr  staff     652 29 jan 08:13 test-client.cpp\n";
//         buffer_data += "-rw-r--r--  1 axellbr  staff   51808 29 jan 16:22 test-client.o\n";
//         buffer_data += "-rw-r--r--  1 axellbr  staff       0 29 jan 15:51 test-json.cpp\n";
//         buffer_data += "-rw-r--r--  1 axellbr  staff     336 29 jan 16:22 test-json.o\n";
//         buffer_data += "-rw-r--r--  1 axellbr  staff     307 29 jan 16:10 test-main.cpp\n";
//         buffer_data += "-rw-r--r--  1 axellbr  staff    1504 29 jan 16:22 test-main.o\n";
//         buffer_data += "-rw-r--r--  1 axellbr  staff    1761 29 jan 16:44 test-tree.cpp\n";
//         buffer_data += "-rw-r--r--  1 axellbr  staff   76392 29 jan 16:39 test-tree.o\n";


//     tree->InitTree( buffer_data.c_str() );
//     std::string json = R"(
//         [
//             {"type":"directory","name":".","contents":[
//                 {"type":"file","name":"test-cli.cpp"},
//                 {"type":"file","name":"test-cli.o"},
//                 {"type":"file","name":"test-client.cpp"},
//                 {"type":"file","name":"test-client.o"},
//                 {"type":"file","name":"test-json.cpp"},
//                 {"type":"file","name":"test-json.o"},
//                 {"type":"file","name":"test-main.cpp"},
//                 {"type":"file","name":"test-main.o"},
//                 {"type":"file","name":"test-tree.cpp"},
//                 {"type":"file","name":"test-tree.o"}
//             ]}
//         ,
//             {"type":"report","directories":1,"files":10}
//         ]
//     )";

//     try {
//         EXPECT_EQ( json, tree->ConvertTreeToJson() );
//     }
//     catch ( const std::string& e ) {
//         std::cerr << "Caught an std::string exception: " << e << std::endl;
//         throw;  // Re-throw the exception to fail the test
//     }
// }
