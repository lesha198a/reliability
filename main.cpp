#include <iostream>
#include "Reliable.h"

int main() {
    Reliable calc;
    double sum = 0;
    for (int i = 0; i < 1000; ++i)
    {
        sum += calc.calculateReliability(0, 1, false);
        sum += calc.calculateReliability(1, 1, false);
        sum += calc.calculateReliability(2, 0.5, false);
        sum += calc.calculateReliability(3, 0.1, false);
    }
    std::cout<<sum/1000<<std::endl;
    sum = 0;
    for (int i = 0; i < 1000; ++i)
    {
        sum += calc.calculateReliability(0, 1, true);
        sum += calc.calculateReliability(1, 1, true);
        sum += calc.calculateReliability(2, 0.5, true);
        sum += calc.calculateReliability(3, 0.1, true);
    }
    std::cout<<sum/1000<<std::endl;
    return 0;
}
