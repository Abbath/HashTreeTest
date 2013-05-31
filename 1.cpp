#include "propersubsets.hpp"
#include <iostream>
#include <fstream>
#include <thread>
#include <future>
#include <ctime>
#include <functional>
class Hash{
public:
    constexpr unsigned operator()(const unsigned long& d){
        return d % 10;
    }
};

typedef std::vector<unsigned long> Data1;
int main(){
    std::ifstream dset("dataset");
    std::ifstream FGset("FG");
    const int dataset_size = 1000;
    const int itemset_size = 20;
    std::vector<Data1> Fg;
    Fg.reserve(dataset_size/2);
    srand(time(NULL));
    for(int i = 0; i < dataset_size/2; ++i){
        Data1 d;
        int b = itemset_size/4;
        d.reserve(b);
        for(int j = 0; j < b; ++j){
            unsigned long a;
            FGset >> a;
            d.push_back(a);
        }
        std::sort(d.begin(),d.end());
        Fg.push_back(d);
    }
    std::vector<Data1> data;
    data.reserve(dataset_size);
    for(int i = 0; i < dataset_size; ++i){
        Data1 d;
        int b =  itemset_size;
        d.reserve(b);
        for(int j = 0; j < b; ++j){
            unsigned long a;
            dset >> a;

            d.push_back(a);
        }
        std::sort(d.begin(),d.end());
        data.push_back(d);
    }
    std::list<std::list<std::vector<unsigned long> > > results;
    ProperSubsets<unsigned long, Hash> p(Fg);
    for(auto it = data.cbegin(); it != data.cbegin() + 100 ; it+=4){
        auto a = std::async(std::launch::async, &ProperSubsets<unsigned long, Hash>::operator(), &p, *it);
        auto b = std::async(std::launch::async, &ProperSubsets<unsigned long, Hash>::operator(), &p, *(it+1));
        auto c = std::async(std::launch::async, &ProperSubsets<unsigned long, Hash>::operator(), &p, *(it+2));
        auto d = std::async(std::launch::async, &ProperSubsets<unsigned long, Hash>::operator(), &p, *(it+3));
        results.push_back(a.get());
        results.push_back(b.get());
        results.push_back(c.get());
        results.push_back(d.get());
    }

    return 0;

}
