#include <gtest/gtest.h>
#include "../include/tree.hpp"


class TreeTest : public ::testing::Test {
protected:
    Tree* tree;

    void SetUp() override {
        tree = new Tree();
    }

    void TearDown() override {
        delete tree;
    }

    void AddChild( Tree* child ) {
        tree->AddChild( child );
    }

};


TEST_F(TreeTest, TreeStartsWithCorrectValues) {
    Tree * tree = new Tree();
    tree->InitTree("drwxr-xr-x  11 ftp      ftp          4096 Aug 31 14:27 pub");
    EXPECT_EQ("root", tree->GetName());
    EXPECT_EQ(true, tree->GetIsDir());
    EXPECT_EQ(nullptr, tree->GetParent());
    EXPECT_EQ(11, tree->GetChildren().size());
}
