#ifndef TREE_HPP
#define TREE_HPP

#include <string>
#include <vector>

class Tree {
public:
    Tree();
    ~Tree();
    Tree* InitTree( const char * buffer_data);
    Tree* NewTree( int depth, std::string name, bool is_dir, Tree* parent, std::string permissions, int nb_links, std::string owner, std::string group, int size, std::string date, std::string time);
    void AddChild( Tree* child );
    void PrintTree(const std::string& prefix = "", bool isLast = true);
    void SetIsDir( bool is_dir );
    std::string GetName() const;
    Tree* GetParent() const;
    bool GetIsDir() const;
    std::vector<Tree*> GetChildren() const;

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
};

#endif  //TREE_HPP

