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
        // Supprimer les espaces de fin de 'name'
        name = name.substr( 0, name.find_last_not_of( '\r' ) + 1 );

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

void Tree::PrintTree() {
    std::cout << this->name_ << std::endl;

    for ( size_t i = 0; i < children_.size(); ++i ) {
        children_[i]->PrintSubTree( "", i == children_.size() - 1 );
    }

    std::cout << std::endl; 
    PrintReport();
}

void Tree::PrintSubTree( const std::string& prefix, bool is_last ) {
    std::string branch = is_last ? "└── " : "├── ";
    std::cout << prefix << branch << this->name_ << std::endl;

    // Construction du préfixe pour les enfants
    std::string child_prefix = prefix + ( is_last ? "    " : "│   " );

    for ( size_t i = 0; i < children_.size(); ++i ) {
        children_[i]->PrintSubTree( child_prefix, i == children_.size() - 1 );
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


std::string Tree::ConvertTreeToJson() {
    // flux de sortie pour manipuler la chaîne de caractères de manière dynamique
    std::pair<int, int> total = UpdateCountDirFileRecursive();
    std::string json = "";

    json += "[\n";
    json += this->ConvertNodeToJson( "  " );
    json += "\n,\n";
        // Mise à jour des compteurs de fichiers et de répertoires
    json += "{\"type\":\"report\",\"directories\":";
    json += std::to_string(total.first);
    json += ",\"files\":";
    json += std::to_string(total.second);
    json +=  "}";
    json += "\n]";

    return json;
}


std::string Tree::ConvertNodeToJson( std::string prefix ) const {
    if ( this->is_dir_ ) {
        return this->ConvertDirToJson( prefix );
    }

    return this->ConvertFileToJson( prefix );
}

std::string Tree::ConvertFileToJson( std::string prefix ) const {
    std::string json = "";

    json += prefix;
    json += "{";
    json += "\"type\":\"file\",";
    json += "\"name\":\"" + this->name_ + "\"";
    json += "}";

    return json;
}

std::string Tree::ConvertDirToJson( std::string prefix ) const {
    std::string json = "";

    json += prefix;
    json += "{";
    json += "\"type\":\"directory\",";
    json += "\"name\":\"" + this->name_ + "\"";

    if ( !children_.empty() ) {
        json += ",\"contents\": [\n";

        for ( size_t i = 0; i < children_.size(); ++i ) {
            json += children_[i]->ConvertNodeToJson( prefix + "  " );

            if ( i < children_.size() - 1 ) {
                json += ",";
            }
            json += "\n";
        }
        json += prefix;
        json += "]";
    }
    json += "}";

    return json;
}

void Tree::PrintTreeJson() {
    std::cout << this->ConvertTreeToJson() << std::endl;
}

std::pair<int, int> Tree::UpdateCountDirFileRecursive() {

    std::pair<int, int> nb_dir_file = std::make_pair( 0, 0 );

    if ( this->is_dir_ ) {
        nb_dir_file.first = 1;
        for ( const auto& child : children_ ) {
            std::pair<int, int> nb_dir_file_child = child->UpdateCountDirFileRecursive();
            // Cumul des compteurs de fichiers et de répertoires
            nb_dir_file.first += nb_dir_file_child.first;
            nb_dir_file.second += nb_dir_file_child.second;
        }
    }
    else {
        nb_dir_file.second = 1;
    }

    return nb_dir_file;
}

void Tree::PrintReport() {
    std::pair<int, int> total =  UpdateCountDirFileRecursive();
    std::cout << total.first << " répertoires, ";
    std::cout << total.second << " fichiers" << std::endl;
}