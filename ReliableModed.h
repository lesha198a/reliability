//
// Created by oleksii on 1/19/23.
//

#ifndef PROGRAM_RELIABLEMODED_H
#define PROGRAM_RELIABLEMODED_H

#include <vector>
#include <set>
#include <map>
#include <string>
#include "Redistribution.h"

class ReliableModed
{
public:
    ReliableModed();

    double calculateReliability(size_t failures, double percentage, bool redistribution);

    std::map<std::string, size_t> getStatistics();
    void resetStatistic();
    void printRedistributionTable() { mRedistribution.printTable(); }

private:
    //count with skipped numbers of elements on circuit (aka put max number from circuit)
    static size_t ACount() { return 4; }
    static size_t BCount() { return 5; }
    static size_t CCount() { return 6; }
    static size_t DCount() { return 8; }
    static size_t MCount() { return 3; }
    static size_t PrCount() { return 6; }

    static std::vector<size_t> ASkipped() { return {}; }
    static std::vector<size_t> BSkipped() { return {}; }
    static std::vector<size_t> CSkipped() { return {}; }
    static std::vector<size_t> DSkipped() { return {4}; }
    static std::vector<size_t> MSkipped() { return {}; }
    static std::vector<size_t> PrSkipped() { return {}; }

    size_t getModelSize() const;

    static size_t factorialTriple(size_t dividend, size_t divisor, size_t divisor2,
                                  double resScale);

    std::vector<std::set<size_t>> getFailurePositions(size_t vecCount, size_t failures,
                                                      size_t modelSize) const;

    std::vector<bool> getStateFrom(const std::set<size_t> &failurePoses, size_t modelSize);
    void setState(const std::vector<bool> &state, size_t modelSize);
    double getProbability();
    int setModulesFromState(size_t modelSize, int iState, const std::vector<bool> &localState,
                            const std::vector<size_t> &skipped, std::vector<bool> &vecToSet,
                            size_t count) const;
    double getModuleReliabilityState(size_t count, const std::vector<size_t> &skipped,
                                     const std::vector<bool> &vec, double reliability) const;

    int statisticForModule(size_t count, const std::vector<size_t> &skipped,
                           std::map<std::string, size_t> &res, int i,
                           const std::string &name) const;
    void appendStageFailureStatistic(std::vector<bool> state);
    void finishStageStatistic();
    void resetStageStatistic(size_t modelSize);

    bool func1()
    {
        return (D(1) or D(2) or D(3)) and (C(1) or C(2) or C(3)) and (B(1) or B(2))
               and (Pr(1) or Pr(2) or Pr(3) or Pr(4)) and (A(1) or A(2));
    }

    bool func2()
    {
        return (A(1) or A(2)) and (M(1) or M(2))
               and (C(4) and (D(5) or D(6)) or A(3) and (B(4) or B(5)));
    }

    bool func3()
    {
        return (A(4) and (B(3) or B(5) or B(5) and A(3) and B(4)) or A(3) and (B(4) or B(5)))
               and (Pr(5) or Pr(6) or (C(5) or C(6)) and (D(7) or D(8)));
    }

    bool func4() { return (A(1) or A(2)) and (M(3) or M(1)); }

    bool mainFunc() { return func1() and func2() and func3() and func4(); }

    void resetStateVecs()
    {
        adapters.clear();
        adapters.resize(ACount(), true);

        buses.clear();
        buses.resize(BCount(), true);

        controllers.clear();
        controllers.resize(CCount(), true);

        detectors.clear();
        detectors.resize(DCount(), true);

        mainlines.clear();
        mainlines.resize(MCount(), true);

        processors.clear();
        processors.resize(PrCount(), true);
    }

    //indexation starts from 1
    bool A(size_t i);
    bool B(size_t i);
    bool C(size_t i);
    bool D(size_t i);
    bool M(size_t i);
    bool Pr(size_t i);

    std::vector<bool> adapters;    //A
    std::vector<bool> buses;       //B
    std::vector<bool> controllers; //C
    std::vector<bool> detectors;   //D
    std::vector<bool> mainlines;   //M
    std::vector<bool> processors;  //Pr

    const double adaptersReliability = 4.2e-4;    //A
    const double busesReliability = 1.4e-5;       //B
    const double controllersReliability = 3.1e-4; //C
    const double detectorsReliability = 2.2e-5;   //D
    const double mainlinesReliability = 1.3e-4;   //M
    const double processorsReliability = 6.2e-4;  //Pr

    std::vector<size_t> mStageStatistic;
    std::vector<size_t> mTotalStatistic;

    Redistribution mRedistribution;
};

#endif //PROGRAM_RELIABLEMODED_H
