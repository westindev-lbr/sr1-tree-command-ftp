#include "../include/tree.hpp"
#include <iostream>
#include <sstream>

Tree::Tree() {
    depth_ = 0;
    name_ = ".";
    is_dir_ = true;
    parent_ = nullptr;
    children_ = std::vector<Tree*>();
}

Tree::~Tree() {
    for ( auto child : children_ ) {
        delete child;
    }
}

Tree* Tree::InitTree( const char* buffer_data ) {

    std::istringstream iss( buffer_data );
    std::string line;

    // Lire chaque ligne du buffer_data
    while ( std::getline( iss, line ) ) {
        std::istringstream line_stream( line );
        std::string permissions, owner, group, month, day, time, name;
        int nb_links, size;

        // Lire chaque champ de la ligne
        line_stream >> permissions >> nb_links >> owner >> group >> size >> month >> day >> time;
        std::getline( line_stream, name );  // Lire le reste de la ligne dans 'name'

        // Formatter la date
        std::string date = month + " " + day;

        // Supprimer les espaces de début de 'name'
        name = name.substr( name.find_first_not_of( ' ' ) );

        // Créer un nouveau noeud
        Tree* new_node = NewTree(
            depth_ + 1,
            name,
            ( permissions[0] == 'd' ),
            this,
            permissions,
            nb_links,
            owner,
            group,
            size,
            date,
            time
        );

        // Ajouter le nouveau noeud à l'arbre
        AddChild( new_node );

    }
    return this;
}

Tree* Tree::NewTree( int depth, std::string name, bool is_dir, Tree* parent, std::string permissions, int nb_links, std::string owner, std::string group, int size, std::string date, std::string time ) {
    Tree* new_tree = new Tree();
    new_tree->depth_ = depth;
    new_tree->name_ = name;
    new_tree->is_dir_ = is_dir;
    new_tree->parent_ = parent;
    new_tree->permissions_ = permissions;
    new_tree->nb_links_ = nb_links;
    new_tree->owner_ = owner;
    new_tree->group_ = group;
    new_tree->size_ = size;
    new_tree->date_ = date;
    new_tree->time_ = time;
    return new_tree;
}


void Tree::AddChild( Tree* child ) {
    children_.push_back( child );
}

void Tree::PrintTree( const std::string& prefix, bool isLast ) {
    std::string branch = isLast ? "└── " : "├── ";
    std::cout << prefix << branch << this->name_ << std::endl;

    // Construction du préfixe pour les enfants
    std::string childPrefix = prefix + ( isLast ? "    " : "│   " );

    for ( size_t i = 0; i < children_.size(); ++i ) {
        children_[i]->PrintTree( childPrefix, i == children_.size() - 1 );
    }
}

Tree* Tree::GetParent() const {
    return parent_;
}

std::string Tree::GetName() const {
    return name_;
}

bool Tree::GetIsDir() const {
    return is_dir_;
}

void Tree::SetIsDir( bool is_dir_condition ) {
    is_dir_ = is_dir_condition;
}

std::vector<Tree*> Tree::GetChildren() const {
    return children_;
}