#ifndef HASHTREE_HPP
#define HASHTREE_HPP

#include <vector>
#include <set>
#include <list>
#include <algorithm>
#include <boost/timer.hpp>

template<typename Data, class Hasher>
class HashTree
{
public:
    HashTree();
    ~HashTree();
    void insert(const std::vector<Data>& d);
    bool contains(const std::vector<Data>& d) const;
    unsigned int getCap() const {return capacity;}
    void erase();
private:
    static const int size = 10;
    unsigned capacity;
    struct Node{
        std::list<std::vector<Data>> data;
        std::vector<Node*> children;
        Node();
    };
    Node * root;
    Hasher hash;
    void erase_rec(Node* p);
};

template<typename Data, class Hasher>
HashTree<Data, Hasher>::HashTree() :  capacity(0), root(new Node)
{
    root->children.resize(size);
}

template<typename Data, class Hasher>
HashTree<Data, Hasher>::~HashTree(){
    erase();
}

template<typename Data, class Hasher>
HashTree<Data, Hasher>::Node::Node() : children(size){}

template<typename Data, class Hasher>
void HashTree<Data, Hasher>::insert(const std::vector<Data> &d){
    Node * p = root;
    for(auto i = d.begin(); i !=  d.end(); ++i){
        unsigned h = hash( *i );
        if( p->children[h] == nullptr){
            p->children[h] = new Node;
            capacity++;
        }
        if( i == d.end()-1  ){
            p->children[h]->data.push_back(d);
            return;
        }else{
            p = p->children[h];
        }
    }
}

template<typename Data, class Hasher>
bool HashTree<Data, Hasher>::contains(const std::vector<Data> &d) const{
    Node * p = root;
    for(auto i = d.begin(); i !=  d.end(); ++i){
        unsigned h = hash( *i );
        if( p->children[h] == nullptr){
            return false;
        }else{
            p = p->children[h];
        }
    }
    return ( p->data.end() != std::find(p->data.begin(), p->data.end(),d));
}

template<typename Data, class Hasher>
void HashTree<Data, Hasher>::erase(){
    erase_rec(root);
}

template<typename Data, class Hasher>
void HashTree<Data, Hasher>::erase_rec(Node *p){
    bool empty = true;
    if( nullptr != p ){
        for( auto it = p->children.begin(); it != p->children.end(); ++it){
            if( *it != nullptr){
                empty = false;
                break;
            }
        }
        if(empty){
            if(p) delete p;
        }else{
            for(int i = 0; i< size;++i)
                erase_rec(p->children[i]);
        }
    }
}
#endif // HASHTREE_HPP
