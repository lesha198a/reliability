#include <iostream>
#include <map>
#include <ctime>
#include <fstream>
#include "Reliable.h"

void printStat(const std::map<std::string, size_t> &stat)
{
    for (const auto &item : stat) {
        std::cout << item.first << ": " << item.second << std::endl;
    }
}

void printStatFile(const std::map<std::string, size_t> &stat, std::ofstream &stream)
{
    for (const auto &item : stat) {
        stream << item.first << ";" << item.second<< ";"  << std::endl;
    }
}

int main()
{
    std::srand(std::time(nullptr));
    Reliable calc;
    calc.printRedistributionTable();
    std::cout<<"\n\n";
    size_t n = 100000;
    double sum1 = 0;
    for (int i = 0; i < n; ++i) {
        sum1 += calc.calculateReliability(0, 1, false);
        sum1 += calc.calculateReliability(1, 1, false);
        sum1 += calc.calculateReliability(2, 0.5, false);
        sum1 += calc.calculateReliability(3, 0.1, false);
    }
    double Q1 = sum1 / n;
    std::cout << "Without active redistribution:" << std::endl;
    std::cout << "Q = " << Q1 << std::endl;
    std::cout << "P = " << 1 - Q1 << std::endl;
    auto stat1 = calc.getStatistics();
    printStat(stat1);

    std::ofstream passiveCsv("passive.csv");
    printStatFile(stat1, passiveCsv);
    passiveCsv.close();

    calc.resetStatistic();
    double sum2 = 0;
    for (int i = 0; i < n; ++i) {
        sum2 += calc.calculateReliability(0, 1, true);
        sum2 += calc.calculateReliability(1, 1, true);
        sum2 += calc.calculateReliability(2, 0.5, true);
        sum2 += calc.calculateReliability(3, 0.1, true);
    }
    double Q2 = sum2 / n;
    std::cout << "\n\nWithout active redistribution:" << std::endl;
    std::cout << "Q = " << Q2 << std::endl;
    std::cout << "P = " << 1 - Q2 << std::endl;
    auto stat2 = calc.getStatistics();
    printStat(stat2);

    std::ofstream activeCsv("active.csv");
    printStatFile(stat2, activeCsv);
    activeCsv.close();
    return 0;
}
