//
// Created by lesha on 14.01.2023.
//

#ifndef PROGRAM_RELIABLE_H
#define PROGRAM_RELIABLE_H

#include <vector>
#include <set>
#include <map>
#include "Redistribution.h"

class Reliable
{
public:
    Reliable();

    double calculateReliability(size_t failures, double percentage, bool redistribution);

    std::map<std::string, size_t> getStatistics();
    void resetStatistic();

private:
    //count with skipped numbers of elements on circuit (aka put max number from circuit)
    static size_t ACount() { return 3; }
    static size_t BCount() { return 4; }
    static size_t CCount() { return 6; }
    static size_t DCount() { return 8; }
    static size_t MCount() { return 2; }
    static size_t PrCount() { return 6; }

    static std::vector<size_t> ASkipped() { return {2}; }
    static std::vector<size_t> BSkipped() { return {}; }
    static std::vector<size_t> CSkipped() { return {3}; }
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
                           std::map<std::string, size_t> &res, int i, const std::string& name) const;
    void appendStageFailureStatistic(std::vector<bool> state);
    void finishStageStatistic();
    void resetStageStatistic(size_t modelSize);

    bool func1()
    {
        return D(2) and (C(1) or C(2)) and (B(1) or B(2)) and (Pr(1) or Pr(2)) and A(1) and M(1);
    }

    bool func2()
    {
        return D(3) and C(2) and (B(1) or B(2))
               and (Pr(3) or (A(1) and M(1) and C(4) and (D(5) or D(6))));
    }

    bool func3()
    {
        return D(1) and C(1) and (B(1) or B(2))
               and (Pr(4) or (A(1) and M(2) and A(3) and B(4) and C(6) and D(8)));
    }

    bool func4()
    {
        return (D(7) or D(8)) and C(5) and B(3) and (Pr(5) or Pr(6)) and B(4) and A(3) and M(2);
    }

    bool mainFunc() { return func1() and func2() and func3() and func4(); }

    static std::vector<bool> getPrState(const std::vector<bool> &fullState);

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

#endif //PROGRAM_RELIABLE_H
