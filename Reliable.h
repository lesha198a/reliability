
#ifndef PROGRAM_RELIABLE_H
#define PROGRAM_RELIABLE_H

#include <vector>
#include <set>
#include <map>
#include <memory>
#include "Redistribution.h"
#include "CircuitModel.h"

class Reliable
{
public:
    Reliable(std::unique_ptr<CircuitModel> model);

    double calculateReliability(size_t failures, double percentage, bool redistribution);

    std::map<std::string, size_t> getStatistics();
    static void amendStatistics(std::map<std::string, size_t> &mainStat,
                         const std::map<std::string, size_t> &otherStat);
    void resetStatistic();
    std::string printRedistributionTable() { return mRedistribution.printTable(); }

    double getProbabilityOfSuccesfulState();

private:

    static size_t factorialTriple(size_t dividend, size_t divisor, size_t divisor2,
                                  double resScale);

    std::vector<std::set<size_t>> getFailurePositions(size_t vecCount, size_t failures,
                                                      size_t modelSize) const;

    void appendStageFailureStatistic(std::vector<bool> state);

    size_t mRedistribStat = 0;
    size_t mCompleteRedistribStat = 0;
    std::vector<size_t> mTotalStatistic;

    Redistribution mRedistribution;

    std::unique_ptr<CircuitModel> mCircuitModel;
};

#endif //PROGRAM_RELIABLE_H
