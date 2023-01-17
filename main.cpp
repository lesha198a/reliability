#include <iostream>
#include <map>
#include "Reliable.h"

void printStat (const std::map<std::string, size_t>& stat) {
    for (const auto &item : stat) {
        std::cout<<item.first<<": "<<item.second<<std::endl;
    }
}


int main() {
    std::srand(std::time(nullptr));
    Reliable calc;
    double sum = 0;
    for (int i = 0; i < 10; ++i)
    {
        sum += calc.calculateReliability(0, 1, false);
        sum += calc.calculateReliability(1, 1, false);
        sum += calc.calculateReliability(2, 0.5, false);
        sum += calc.calculateReliability(3, 0.1, false);
    }
    std::cout<<sum/10<<std::endl;
    printStat(calc.getStatistics());
    sum = 0;
    for (int i = 0; i < 10000; ++i)
    {
        sum += calc.calculateReliability(0, 1, true);
        sum += calc.calculateReliability(1, 1, true);
        sum += calc.calculateReliability(2, 0.5, true);
        sum += calc.calculateReliability(3, 0.1, true);
    }
    std::cout<<sum/10000<<std::endl;
    return 0;
}
