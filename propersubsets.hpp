#ifndef PROPERSUBSETS_HPP
#define PROPERSUBSETS_HPP
#include <vector>
#include <list>
#include <iostream>
#include <functional>
#include <future>
#include "hashtree.hpp"
#include "hashtree_sup.hpp"
#include "propersubsets.hpp"

template<typename Data, class Hasher>
class ProperSubsets{
    typedef std::vector<Data> Itemset;
    typedef std::vector<Itemset> Dataset;
    typedef std::pair<std::vector<Data>,unsigned long> ItemsetSup;
    typedef std::vector<ItemsetSup> DatasetSup;
public:
    ProperSubsets(const DatasetSup &FG);
    std::list<ItemsetSup> operator()(const Itemset&);
    void setFG(const DatasetSup &FG);
private:
    void run(int p,int k,Itemset& t, DatasetSup& t0,std::list<ItemsetSup>& result, const Itemset& data, int q=0,int r=0);
    unsigned int minlen;
    unsigned int maxlen;
    std::vector<ItemsetSup> cache;
    //HashTree<Data,Hasher> tree;
    HashTreeSup<Data,Hasher> treesup;
};

template<typename Data, class Hasher>
ProperSubsets<Data, Hasher>::ProperSubsets(const DatasetSup& FG) : minlen(FG[0].first.size()), maxlen(1), cache(FG){
    Hasher h;
    h(FG[0].first[0]);
    for(const ItemsetSup& x : cache){
        if(x.first.size() > maxlen){
            maxlen = x.first.size();
        }
        if(x.first.size() < minlen){
            minlen = x.first.size();
        }
        treesup.insert( x );
    }
    std::ofstream f("tree");
    treesup.out(f);
    std::cout << treesup.getCap() << std::endl;
}

template<typename Data, class Hasher>
std::list<std::pair<std::vector<Data>,unsigned long> > ProperSubsets<Data, Hasher>::operator ()(const Itemset& X){
    std::list<ItemsetSup> result;
    for(unsigned int i=minlen;i<=maxlen;++i){
        std::vector<Data> tmp(X.size());
        DatasetSup tmp0(maxlen - minlen + 1);
        run(X.size(),i,tmp,tmp0,result,X);
    }
    return result;
}

template<typename Data, class Hasher>
void ProperSubsets<Data, Hasher>::setFG(const DatasetSup &FG){
    if(!std::equal(cache.begin(),cache.end(),FG.begin())){
        cache = FG;
        treesup.erase();
        maxlen = 1;
        minlen = FG[0].first.size();
        for(const ItemsetSup& x : cache){
            if(x.first.size() > maxlen){
                maxlen = x.first.size();
            }
            if(x.first.size() < minlen){
                minlen = x.first.size();
            }
            treesup.insert( x );
        }
    }
}

template<typename Data, typename Hasher>
void ProperSubsets<Data,Hasher>::run(int len, int sublen, Itemset& t, DatasetSup& t0, std::list<ItemsetSup> &result,const Itemset& data, int q, int r){
    if(t0[0].first.size() != minlen){
        for(unsigned int i = 0; i <= maxlen-minlen; ++i){
            t0[i].first.resize(i+minlen);
        }
    }
    if(q == sublen){
        std::copy(t.begin(),t.begin() + sublen,t0[sublen - minlen].first.begin());
        if(auto res = treesup.contains(t0[sublen-minlen].first)){
            t0[sublen - minlen].second = res;
            result.push_back(t0[sublen - minlen]);
        }
    }else{
        for(int i = r; i < len; ++i){
            t[q] = data[i];
            run(len,sublen,t,t0,result,data,q + 1,i + 1);
        }
    }
}
#endif // PROPERSUBSETS_HPP
