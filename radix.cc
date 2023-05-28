#include <string>
#include <thread>
#include <math.h>
#include <algorithm>
#include "radix.h"

// returns the number of digits in uint a
unsigned int NoD(unsigned int a) {
    unsigned int count = 0;
    while(a > 0) {
        a /= 10;
        count++;
    }
    return count;
}

// returns the most significant digit of uint a
unsigned int FirstDigit(unsigned int a) {
    while(a > 9) {
        a /= 10;
    }
    return a;
}

// returns uint a without its most significant digit
unsigned int EliminateFirstDigit(unsigned int a) {
    return a - pow(10, NoD(a)-1) * FirstDigit(a);
}

// https://stackoverflow.com/questions/4261589/how-do-i-split-an-int-into-its-digits
unsigned int GetDigit(unsigned int a, unsigned int index) {
    unsigned int p = 1;
    unsigned int n = NoD(a);
    for(unsigned int i = 0; i < n - index - 1; i++) {
        p *= 10;
    }
    return a / p % 10;
}

bool Greater(unsigned int a, unsigned int b) {
    unsigned int min, shortest;
    if(NoD(a) > NoD(b)) {
        min = NoD(b);
        shortest = b;
    }
    else {
        min = NoD(a);
        shortest = a;
    }
    for(unsigned int i = 0; i < min; i++) {
        if(GetDigit(a, i) < GetDigit(b, i)) {
            return false;
        }
        if(GetDigit(a, i) > GetDigit(b, i)) {
            return true;
        }
    }
    return (shortest != a);
}

void Sorting(std::vector<unsigned int> &list) {
    std::sort(list.begin(), list.end(), [](unsigned int a, unsigned int b) {
        return Greater(b, a);
    });
}

void ParallelRadixSort::msd(std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists, unsigned int cores) { 
    std::vector<std::thread> threads;
    unsigned int buckets = 90;
    for(std::vector<unsigned int>& l : lists) {
        std::vector<std::vector<unsigned int>> list(buckets);
        for(unsigned int i : l) {
            unsigned int e = EliminateFirstDigit(i);
            if(NoD(i) - 1 == NoD(e)) {
                list[(FirstDigit(i) - 1) * 10 + FirstDigit(e)].push_back(i);
            }
            else {
                list[(FirstDigit(i) - 1) * 10].push_back(i);
            }
        }
        l.clear();
        unsigned int count = 0;
        for(unsigned int j = 0; j < buckets; j++) {
            threads.push_back(std::move(std::thread(Sorting, std::ref(list[j]))));
            if(j == buckets - 1 || (j + 1) % cores == 0) {
                for(auto& i : threads) {
                    i.join();
                    l.insert(l.end(), list[count].begin(), list[count].end());
                    count++;
                }
                threads.clear();
            }
        }
    }
}
