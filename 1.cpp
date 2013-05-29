#include "propersubsets.hpp"
#include <iostream>
#include <fstream>
#include <thread>
#include <future>
#include <ctime>
#include <functional>
#include <boost/timer.hpp>
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
    std::list<std::vector<unsigned long>> a,b,c,d;
    for(auto it = data.cbegin(); it != data.cbegin() + 100 ; it+=4){
        std::thread t0(&ProperSubsets<unsigned long, Hash>::operator(), &p, *it, std::ref(a));
        std::thread t1(&ProperSubsets<unsigned long, Hash>::operator(), &p, *(it+1), std::ref(b));
        std::thread t2(&ProperSubsets<unsigned long, Hash>::operator(), &p, *(it+2), std::ref(c));
        std::thread t3(&ProperSubsets<unsigned long, Hash>::operator(), &p, *(it+3), std::ref(d));
        t0.join();
        t1.join();
        t2.join();
        t3.join();
        results.push_back(a);
        results.push_back(b);
        results.push_back(c);
        results.push_back(d);
    }

    return 0;

}
