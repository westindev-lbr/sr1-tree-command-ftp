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
