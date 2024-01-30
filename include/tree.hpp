#ifndef TREE_HPP
#define TREE_HPP

#include <string>
#include <vector>

class Tree {
public:
    Tree();
    ~Tree();
    Tree* InitTree( const char* buffer_data );
    Tree* NewTree(
        int depth,
        std::string name,
        bool is_dir,
        Tree* parent,
        std::string permissions,
        int nb_links,
        std::string owner,
        std::string group,
        int size,
        std::string date,
        std::string time
    );

    void AddChild( Tree* child );
    void PrintTree();
    void PrintTreeJson();
    void SetIsDir( bool is_dir );
    std::string GetName() const;
    Tree* GetParent() const;
    bool GetIsDir() const;
    std::vector<Tree*> GetChildren() const;
    std::pair<int,int> UpdateCountDirFileRecursive();
    std::string ConvertTreeToJson();

private:
    int depth_;
    bool is_dir_;
    std::string permissions_;
    int nb_links_;
    std::string owner_;
    std::string group_;
    int size_;
    std::string date_;
    std::string time_;
    std::string name_;
    Tree* parent_;
    std::vector<Tree*> children_;

    void PrintSubTree( const std::string& prefix = "", bool isLast = true );
    std::string ConvertNodeToJson( std::string prefix ) const;
    std::string ConvertFileToJson( std::string prefix ) const;
    std::string ConvertDirToJson( std::string prefix ) const;
    void PrintReport();   
};

#endif  //TREE_HPP

