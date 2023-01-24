
#include <iostream>
#include <map>
#include <ctime>
#include <fstream>
#include <thread>
#include "Reliable.h"

#include <limits>
#include <sstream>

typedef std::numeric_limits<double> dbl;

void printStat(const std::map<std::string, size_t> &stat)
{
    for (const auto &item : stat) {
        std::cout << item.first << ": " << item.second << std::endl;
    }
}

std::string printStatString(const std::map<std::string, size_t> &stat)
{
    std::stringstream stream;
    for (const auto &item : stat) {
        stream << item.first << ": " << item.second << std::endl;
    }
    return stream.str();
}

void printStatFile(const std::map<std::string, size_t> &stat, std::ofstream &stream)
{
    for (const auto &item : stat) {
        stream << item.first << ";" << item.second << ";" << std::endl;
    }
}

template<typename T, typename = std::enable_if_t<std::is_base_of<CircuitModel, T>::value>>
std::string calculate(size_t ages, const std::string &csvName, bool redistribution)
{
    auto start = std::chrono::steady_clock::now();

    auto calc8 = Reliable(std::make_unique<T>());
    auto calc7 = Reliable(std::make_unique<T>());
    auto calc6 = Reliable(std::make_unique<T>());
    auto calc5 = Reliable(std::make_unique<T>());
    auto calc4 = Reliable(std::make_unique<T>());
    auto calc3 = Reliable(std::make_unique<T>());
    auto calc1 = Reliable(std::make_unique<T>());
    auto calc2 = Reliable(std::make_unique<T>());
    calc1.printRedistributionTable();
    size_t n = (size_t)(ages / 8) * 8;
    double reliabilityOfTrueState = calc1.getProbabilityOfSuccesfulState();
    double sum = 0;
    double sum1 = 0;
    double sum2 = 0;
    double sum3 = 0;
    double sum4 = 0;
    double sum5 = 0;
    double sum6 = 0;
    double sum7 = 0;
    double sum8 = 0;
    std::thread t11([&sum1, &calc1, n, redistribution] {
        for (int i = 0; i < n / 8; ++i) {
            sum1 += calc1.calculateReliability(0, 1, redistribution);
        }
    });
    std::thread t12([&sum2, &calc2, n, redistribution] {
        for (int i = 0; i < n / 8; ++i) {
            sum2 += calc2.calculateReliability(1, 1, redistribution);
        }
    });
    std::thread t13([&sum3, &calc3, n, redistribution] {
        for (int i = 0; i < n / 8; ++i) {
            sum3 += calc3.calculateReliability(2, 0.5, redistribution);
        }
    });
    std::thread t14([&sum4, &calc4, n, redistribution] {
        for (int i = 0; i < n / 8; ++i) {
            sum4 += calc4.calculateReliability(3, 0.1, redistribution);
        }
    });
    std::thread t15([&sum5, &calc5, n, redistribution] {
        for (int i = 0; i < n / 8; ++i) {
            sum5 += calc5.calculateReliability(0, 1, redistribution);
        }
    });
    std::thread t16([&sum6, &calc6, n, redistribution] {
        for (int i = 0; i < n / 8; ++i) {
            sum6 += calc6.calculateReliability(1, 1, redistribution);
        }
    });
    std::thread t17([&sum7, &calc7, n, redistribution] {
        for (int i = 0; i < n / 8; ++i) {
            sum7 += calc7.calculateReliability(2, 0.5, redistribution);
        }
    });
    std::thread t18([&sum8, &calc8, n, redistribution] {
        for (int i = 0; i < n / 8; ++i) {
            sum8 += calc8.calculateReliability(3, 0.1, redistribution);
        }
    });
    t11.join();
    t12.join();
    t13.join();
    t14.join();
    t15.join();
    t16.join();
    t17.join();
    t18.join();
    sum = sum1 + sum2 + sum3 + sum4 + sum5 + sum6 + sum7 + sum8;
    auto stat = calc1.getStatistics();
    Reliable::amendStatistics(stat, calc2.getStatistics());
    Reliable::amendStatistics(stat, calc3.getStatistics());
    Reliable::amendStatistics(stat, calc4.getStatistics());
    Reliable::amendStatistics(stat, calc5.getStatistics());
    Reliable::amendStatistics(stat, calc6.getStatistics());
    Reliable::amendStatistics(stat, calc7.getStatistics());
    Reliable::amendStatistics(stat, calc8.getStatistics());
    double P = sum / (double)n + reliabilityOfTrueState; /*

    std::cout.precision(dbl::max_digits10);
    std::cout<<(double)(sum/(double)n)<<std::endl;*/

    auto end = std::chrono::steady_clock::now();
    auto timeSpan = static_cast<std::chrono::duration<double>>(end - start);
    std::stringstream stream;
    stream.precision(dbl::max_digits10);
    stream << "Redistribution: " << (redistribution ? "true" : "false") << std::endl;
    if (redistribution) {
        stream << "Redistribution table:" << std::endl
               << calc1.printRedistributionTable() << std::endl;
    }
    stream << "Iterations: " << n << std::endl;
    stream << "Duration: " << timeSpan.count() << "s" << std::endl;
    stream << "Q = " << 1 - P << std::endl;
    stream << "P = " << P << std::endl << std::endl;
    stream << printStatString(stat) << std::endl;

    std::ofstream csv(csvName + ".csv");
    printStatFile(stat, csv);
    csv.close();
    return stream.str();
}

int main()
{
    std::string passive;
    std::string active;
    std::string passiveModified;
    std::string activeModified;
    size_t n = 1000000;
    std::thread tPassive([&passive, n] { passive = calculate<OrigCircuit>(n, "passive", false); });
    std::thread tActive([&active, n] { active = calculate<OrigCircuit>(n, "active", true); });
    std::thread tPassiveModified([&passiveModified, n] {
        passiveModified = calculate<ModifiedCircuit>(n, "passiveModified", false);
    });
    std::thread tActiveModified([&activeModified, n] {
        activeModified = calculate<ModifiedCircuit>(n, "activeModified", true);
    });

    tPassive.join();
    std::cout << passive << std::endl << std::endl;

    tActive.join();
    std::cout << active << std::endl << std::endl;

    tPassiveModified.join();
    std::cout << passiveModified << std::endl << std::endl;

    tActiveModified.join();
    std::cout << activeModified << std::endl << std::endl;

    return 0;
}
