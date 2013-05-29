#ifndef SUBSETGENERATOR_HPP
#define SUBSETGENERATOR_HPP

#include <vector>
#include <list>
#include <algorithm>

template<typename Data>
class SubsetGenerator
{
public:
    SubsetGenerator();
    ~SubsetGenerator();
    SubsetGenerator(const std::vector<Data> &d, int _n);
    std::vector<Data> getNextSubset();
private:
    int n;
    bool first,first0;
    std::vector<Data> data;
    std::list<std::vector<Data>> subsets;
    void subset(int p,int k,std::vector<Data>& t, std::vector<std::vector<Data> >& t0, int q=0,int r=0);
};

template<typename Data>
SubsetGenerator<Data>::SubsetGenerator(){
    subsets.reserve( (1 << n) - 1);
}

template<typename Data>
SubsetGenerator<Data>::~SubsetGenerator(){}

template<typename Data>
SubsetGenerator<Data>::SubsetGenerator(const std::vector<Data>& d, int _n) : n(_n) ,first(true), first0(true), data(d) {}

template<typename Data>
std::vector<Data> SubsetGenerator<Data>::getNextSubset(){
    if(first){
        for(int i=1;i<=n;++i){
            std::vector<Data> tmp(n);
            std::vector<std::vector<Data> > tmp0(n);
            subset(n,i,tmp,tmp0);
        }
        first = false;
    }
    if(subsets.empty()){
        std::vector<Data> a(0);
        return a;
    }
    auto a =  subsets.back();
    subsets.pop_back();
    return a;
}

template<typename Data>
void SubsetGenerator<Data>::subset(int len, int sublen,std::vector<Data>& t, std::vector<std::vector<Data> >& t0, int q, int r){
    if(t0[0].size() != 1){
        for(int i = 0; i < len; ++i){
            t0[i].resize(i+1);
        }
    }
    if(q == sublen){
        std::copy(t.begin(),t.begin() + sublen,t0[sublen - 1].begin());
        subsets.push_back(t0[sublen - 1]);
    }else{
        for(int i = r; i < len; ++i){
            t[q] = data[i];
            subset(len,sublen,t,t0,q + 1,i + 1);
        }
    }
}


#endif // SUBSETGENERATOR_HPP
