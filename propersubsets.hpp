#ifndef PROPERSUBSETS_HPP
#define PROPERSUBSETS_HPP
#include <vector>
#include <list>
#include <iostream>
#include <functional>
#include "hashtree.hpp"
#include "propersubsets.hpp"
//#include "subsetgenerator.hpp"
#include <boost/timer.hpp>

template<typename Data, class Hasher>
class ProperSubsets{
    typedef std::vector<Data> Itemset;
    typedef std::vector<Itemset> Dataset;
public:
    ProperSubsets(const Dataset& FG);
    std::list<Itemset > operator()(const Itemset&);
    void setFG(const std::vector<Itemset > &FG);
private:
    void run(int p,int k,Itemset& t, std::vector<Itemset >& t0,std::list<Itemset>& result, const Itemset& data, int q=0,int r=0);
    std::vector<Itemset > cache;
    HashTree<Data,Hasher> tree;
};

template<typename Data, class Hasher>
ProperSubsets<Data, Hasher>::ProperSubsets(const Dataset& FG) : cache(FG){
    Hasher h;
    h(FG[0][0]);
    for(const Itemset& x : cache){
        tree.insert( x );
    }
    std::cout << tree.getCap() << std::endl;
}

template<typename Data, class Hasher>
std::list<std::vector<Data> > ProperSubsets<Data, Hasher>::operator ()(const Itemset& X){
    static boost::timer t1;
    t1.restart();
    std::list<Itemset> result;
    for(unsigned int i=1;i<=X.size();++i){
        std::vector<Data> tmp(X.size());
        std::vector<std::vector<Data> > tmp0(X.size());
        run(X.size(),i,tmp,tmp0,result,X);
    }
    //SubsetGenerator<Data> subgen(X, X.size(), tree);
    //subgen.getResults();
    //auto subset = subgen.getNextSubset();
//    while( subset.size() > 0){
//        if( tree.contains(subset)){
//            result.push_back(subset);
//        }
//        subset = subgen.getNextSubset();
//    }
   // if(result.size() > 0) std::cout << "We have one!" << std::endl;
    std::cout << t1.elapsed() << std::endl;
    return result;
}

template<typename Data, class Hasher>
void ProperSubsets<Data, Hasher>::setFG(const Dataset &FG){
    if(!std::equal(cache.begin(),cache.end(),FG.begin())){
        cache = FG;
        tree.erase();
        for(const Itemset& x : cache){
            tree.insert( x );
        }
    }
}

template<typename Data, typename Hasher>
void ProperSubsets<Data,Hasher>::run(int len, int sublen, Itemset& t, Dataset& t0, std::list<Itemset> &result,const Itemset& data, int q, int r){
    if(t0[0].size() != 1){
        for(int i = 0; i < len; ++i){
            t0[i].resize(i+1);
        }
    }
    if(q == sublen){
        std::copy(t.begin(),t.begin() + sublen,t0[sublen - 1].begin());
        //subsets.push_back(t0[sublen - 1]);
        if(tree.contains(t0[sublen-1])){
            result.push_back(t0[sublen - 1]);
        }
    }else{
        for(int i = r; i < len; ++i){
            t[q] = data[i];
            run(len,sublen,t,t0,result,data,q + 1,i + 1);
        }
    }
}
#endif // PROPERSUBSETS_HPP
