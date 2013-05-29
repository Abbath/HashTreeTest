#ifndef PROPERSUBSETS_HPP
#define PROPERSUBSETS_HPP
#include <vector>
#include <list>
#include <iostream>
#include <functional>
#include "hashtree.hpp"
#include "propersubsets.hpp"
#include "subsetgenerator.hpp"
#include <boost/timer.hpp>


template<typename Data, class Hasher>
class ProperSubsets{
public:
    ProperSubsets(const std::vector<std::vector<Data>>& FG);
    std::list<std::vector<Data> > operator()(const std::vector<Data>&, std::list<std::vector<Data>>& result);
    void setFG(const std::vector<std::vector<Data> > &FG);
private:
    std::vector<std::vector<Data> > cache;
    HashTree<Data,Hasher> tree;
};

template<typename Data, class Hasher>
ProperSubsets<Data, Hasher>::ProperSubsets(const std::vector<std::vector<Data>>& FG) : cache(FG){
    Hasher h;
    h(FG[0][0]);
    for(const std::vector<Data>& x : cache){
        tree.insert( x );
    }
    std::cout << tree.getCap() << std::endl;
}

template<typename Data, class Hasher>
std::list<std::vector<Data> > ProperSubsets<Data, Hasher>::operator ()(const std::vector<Data>& X, std::list<std::vector<Data>>& result){
    static boost::timer t1;
    t1.restart();
    SubsetGenerator<Data> subgen(X, X.size());
    auto subset = subgen.getNextSubset();
    while( subset.size() > 0){
        if( tree.contains(subset)){
            result.push_back(subset);
        }
        subset = subgen.getNextSubset();
    }
    if(result.size() > 0) std::cout << "We have one!" << std::endl;
    std::cout << t1.elapsed() << std::endl;
    return result;
}

template<typename Data, class Hasher>
void ProperSubsets<Data, Hasher>::setFG(const std::vector<std::vector<Data>> &FG){
    if(!std::equal(cache.begin(),cache.end(),FG.begin())){
        cache = FG;
        tree.erase();
        for(const std::vector<Data>& x : cache){
            tree.insert( x );
        }
    }
}

#endif // PROPERSUBSETS_HPP
