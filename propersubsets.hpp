#ifndef PROPERSUBSETS_HPP
#define PROPERSUBSETS_HPP

#include <vector>
#include <list>
#include <functional>
#include <future>
#include "hashtree_sup.hpp"

template<typename Data, class Hasher>
/*!
 * \brief The ProperSubsets class
 */
class ProperSubsets{
    typedef std::vector<Data> Itemset;
    typedef std::vector<Itemset> Dataset;
    typedef std::pair<std::vector<Data>,unsigned long> ItemsetSup;
    typedef std::vector<ItemsetSup> DatasetSup;
public:
    ProperSubsets(const DatasetSup &FG, unsigned int);
    std::list<ItemsetSup> operator()(const Itemset&);
    void setFG(const DatasetSup &FG);
private:
    void run_time(int p,int k,Itemset& t, DatasetSup& t0,std::list<ItemsetSup>& result, const Itemset& data, int q=0,int r=0);
    void run_memory(const Itemset& data, std::list<ItemsetSup>& result);
    unsigned int minlen;
    unsigned int maxlen;
    std::vector<ItemsetSup> cache;
    HashTreeSup<Data,Hasher> treesup;
};

template<typename Data, class Hasher>
/*!
 * \brief ProperSubsets<Data, Hasher>::run_memory
 * \param data
 * \param result
 */
void ProperSubsets<Data, Hasher>::run_memory(const ProperSubsets::Itemset &data, std::list<ProperSubsets::ItemsetSup> &result){
    result = treesup.getSubsets(data);
}

template<typename Data, class Hasher>
/*!
 * \brief ProperSubsets<Data, Hasher>::ProperSubsets
 * \param FG
 * \param s
 */
ProperSubsets<Data, Hasher>::ProperSubsets(const DatasetSup& FG, unsigned int s = 13) : minlen(FG[0].first.size()), maxlen(1), cache(FG), treesup(s){
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
/*!
 * \brief ProperSubsets<Data, Hasher>::operator ()
 * \param X
 * \return
 */
std::list<std::pair<std::vector<Data>,unsigned long> > ProperSubsets<Data, Hasher>::operator ()(const Itemset& X){
    std::list<ItemsetSup> result;
    /*for(unsigned int i=minlen;i<=maxlen;++i){
        std::vector<Data> tmp(X.size());
        DatasetSup tmp0(maxlen - minlen + 1);
        run_time(X.size(),i,tmp,tmp0,result,X);
    }*/
    run_memory(X,result);
    return result;
}

template<typename Data, class Hasher>
/*!
 * \brief ProperSubsets<Data, Hasher>::setFG
 * \param FG
 */
void ProperSubsets<Data, Hasher>::setFG(const DatasetSup &FG){
    if(cache != FG){
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
/*!
 * \brief ProperSubsets<Data, Hasher>::run_time
 * \param len
 * \param sublen
 * \param t
 * \param t0
 * \param result
 * \param data
 * \param q
 * \param r
 */
void ProperSubsets<Data,Hasher>::run_time(int len, int sublen, Itemset& t, DatasetSup& t0, std::list<ItemsetSup> &result,const Itemset& data, int q, int r){
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
            run_time(len,sublen,t,t0,result,data,q + 1,i + 1);
        }
    }
}
#endif // PROPERSUBSETS_HPP
