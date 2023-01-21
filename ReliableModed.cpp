#include "ReliableModed.h"

#include <ctime>
#include <algorithm>
#include <set>
#include <iostream>
#include <sstream>
#include "ReliableModed.h"

ReliableModed::ReliableModed()
{
    mRedistribution.setProcessorsCount(PrCount(), PrSkipped());
    mRedistribution.setProcessorParams(1, 45, 90);
    mRedistribution.setProcessorParams(2, 60, 90);
    mRedistribution.setProcessorParams(3, 50, 90);
    mRedistribution.setProcessorParams(4, 60, 90);
    mRedistribution.setProcessorParams(5, 20, 50);
    mRedistribution.setProcessorParams(6, 40, 50);
    resetStatistic();
}

bool ReliableModed::A(size_t i)
{
    if (i > adapters.size()) {
        exit(1);
    }
    return adapters[i - 1];
}

bool ReliableModed::B(size_t i)
{
    if (i > buses.size()) {
        exit(1);
    }
    return buses[i - 1];
}

bool ReliableModed::C(size_t i)
{
    if (i > controllers.size()) {
        exit(1);
    }
    return controllers[i - 1];
}

bool ReliableModed::M(size_t i)
{
    if (i > mainlines.size()) {
        exit(1);
    }
    return mainlines[i - 1];
}

bool ReliableModed::Pr(size_t i)
{
    if (i > processors.size()) {
        exit(1);
    }
    return processors[i - 1];
}

bool ReliableModed::D(size_t i)
{
    if (i > detectors.size()) {
        exit(1);
    }
    return detectors[i - 1];
}

double ReliableModed::calculateReliability(size_t failures, double percentage, bool redistribution)
{
    double result = 0;
    auto modelSize = getModelSize();
    size_t vecCount = factorialTriple(modelSize, failures, modelSize - failures, percentage);
    auto failurePosVecs = getFailurePositions((size_t)vecCount, failures, modelSize);

    for (const auto &failurePoses : failurePosVecs) {
        auto stateVec = getStateFrom(failurePoses, modelSize);
        setState(stateVec, modelSize);
        bool sysState = mainFunc();
        if (redistribution && !sysState) {
            bool redistributed = mRedistribution.redistribute(processors);
            if (redistributed) {
                auto stateCopy = stateVec;
                for (int i = stateCopy.size() - (PrCount() - PrSkipped().size());
                     i < stateCopy.size(); ++i) {
                    stateCopy[i] = true;
                }
                setState(stateCopy, modelSize);
                sysState = mainFunc();
            }
        }
        if (sysState) {
            setState(stateVec, modelSize);
            result += getProbability();
        } else {
            appendStageFailureStatistic(stateVec);
        }
    }
    return result / percentage;
}

size_t ReliableModed::getModelSize() const
{
    return ACount() + BCount() + CCount() + MCount() + PrCount() + DCount()
           - (ASkipped().size() + BSkipped().size() + CSkipped().size() + DSkipped().size()
              + MSkipped().size() + PrSkipped().size());
}

size_t ReliableModed::factorialTriple(size_t dividend, size_t divisor, size_t divisor2,
                                      double resScale)
{
    size_t dividendFixed = dividend;
    if (dividend < 2) {
        dividendFixed = 1;
    }
    size_t divisorFixed = divisor;
    if (divisor < 2) {
        divisorFixed = 1;
    }
    size_t divisor2Fixed = divisor2;
    if (divisor2 < 2) {
        divisor2Fixed = 1;
    }
    size_t dividendLeft = divisorFixed + 1;
    size_t dividendRight = dividendFixed;
    size_t divisorLeft = dividendFixed + 1;
    size_t divisorRight = divisorFixed;
    size_t divisor2Left1 = 1;
    size_t divisor2Right1 = 1;
    size_t divisor2Left2 = 1;
    size_t divisor2Right2 = divisor2Fixed;
    if (divisor2Fixed >= dividendLeft) {
        divisor2Right1 = dividendLeft - 1;
        divisor2Left2 = dividendRight + 1;
        dividendLeft = divisor2Fixed + 1;
    }
    size_t res1 = 1;
    for (size_t i = dividendLeft; i <= dividendRight; ++i) {
        res1 *= i;
    }
    size_t res2 = 1;
    for (size_t i = divisorLeft; i <= divisorRight; ++i) {
        res2 *= i;
    }
    size_t res3 = 1;
    for (size_t i = divisor2Left1; i <= divisor2Right1; ++i) {
        res3 *= i;
    }
    size_t res4 = 1;
    for (size_t i = divisor2Left2; i <= divisor2Right2; ++i) {
        res4 *= i;
    }
    return static_cast<size_t>(resScale * ((res1) / (res2 * res3 * res4)));
}

std::vector<std::set<size_t>> ReliableModed::getFailurePositions(size_t vecCount, size_t failures,
                                                                 size_t modelSize) const
{
    std::vector<std::set<size_t>> res;
    for (int i = 0; i < vecCount; ++i) {
        std::set<size_t> positions;
        while (positions.size() <= failures) {
            size_t pos = std::rand() % modelSize;
            positions.insert(pos);
        }
        res.push_back(positions);
    }
    return res;
}

std::vector<bool> ReliableModed::getStateFrom(const std::set<size_t> &failurePoses,
                                              size_t modelSize)
{
    std::vector<bool> res;
    res.resize(modelSize, true);
    for (const auto &failurePos : failurePoses) {
        res[failurePos] = false;
    }
    return res;
}

void ReliableModed::setState(const std::vector<bool> &state, size_t modelSize)
{
    resetStateVecs();
    int i = setModulesFromState(modelSize, 0, state, ASkipped(), adapters, ACount());
    i = setModulesFromState(modelSize, i, state, BSkipped(), buses, BCount());
    i = setModulesFromState(modelSize, i, state, CSkipped(), controllers, CCount());
    i = setModulesFromState(modelSize, i, state, DSkipped(), detectors, DCount());
    i = setModulesFromState(modelSize, i, state, MSkipped(), mainlines, MCount());
    setModulesFromState(modelSize, i, state, PrSkipped(), processors, PrCount());
}

int ReliableModed::setModulesFromState(size_t modelSize, int iState,
                                       const std::vector<bool> &localState,
                                       const std::vector<size_t> &skipped,
                                       std::vector<bool> &vecToSet, size_t count) const
{
    for (size_t j = 0; j < count && iState < modelSize; ++j) {
        if (std::find(skipped.begin(), skipped.end(), j + 1) != skipped.end()) {
            continue;
        }
        vecToSet[j] = localState[iState];
        iState++;
    }
    return iState;
}

double ReliableModed::getProbability()
{
    double res = 1;

    res *= getModuleReliabilityState(ACount(), ASkipped(), adapters, adaptersReliability);
    res *= getModuleReliabilityState(BCount(), BSkipped(), buses, busesReliability);
    res *= getModuleReliabilityState(CCount(), CSkipped(), controllers, controllersReliability);
    res *= getModuleReliabilityState(DCount(), DSkipped(), detectors, detectorsReliability);
    res *= getModuleReliabilityState(MCount(), MSkipped(), mainlines, mainlinesReliability);
    res *= getModuleReliabilityState(PrCount(), PrSkipped(), processors, processorsReliability);

    return res;
}

double ReliableModed::getModuleReliabilityState(size_t count, const std::vector<size_t> &skipped,
                                                const std::vector<bool> &vec,
                                                double reliability) const
{
    double res = 1;
    for (int i = 0; i < count; ++i) {
        if (std::find(skipped.begin(), skipped.end(), i + 1) != skipped.end()) {
            continue;
        }
        double applicant = vec[i] ? 1 : 0;
        res *= applicant * (1 - reliability) + (1 - applicant) * reliability;
    }
    return res;
}

void ReliableModed::appendStageFailureStatistic(std::vector<bool> state)
{
    for (int i = 0; i < state.size(); ++i) {
        if (!state[i]) {
            mTotalStatistic[i]++;
        }
    }
}

void ReliableModed::finishStageStatistic()
{
    for (int i = 0; i < mTotalStatistic.size(); ++i) {
        mTotalStatistic[i] += mStageStatistic[i];
    }
}

void ReliableModed::resetStageStatistic(size_t modelSize)
{
    mStageStatistic.clear();
    mStageStatistic.resize(modelSize, 0);
}

std::map<std::string, size_t> ReliableModed::getStatistics()
{
    std::map<std::string, size_t> res;

    int i = statisticForModule(ACount(), ASkipped(), res, 0, "A");
    i = statisticForModule(BCount(), BSkipped(), res, i, "B");
    i = statisticForModule(CCount(), CSkipped(), res, i, "C");
    i = statisticForModule(DCount(), DSkipped(), res, i, "D");
    i = statisticForModule(MCount(), MSkipped(), res, i, "M");
    statisticForModule(PrCount(), PrSkipped(), res, i, "Pr");

    return res;
}

int ReliableModed::statisticForModule(size_t count, const std::vector<size_t> &skipped,
                                      std::map<std::string, size_t> &res, int i,
                                      const std::string &name) const
{
    std::stringstream stream;
    for (int j = 0; j < count; ++j) {
        if (std::find(skipped.begin(), skipped.end(), j + 1) != skipped.end()) {
            continue;
        }
        stream << name << j + 1;
        auto resName = stream.str();
        stream.str("");
        res[resName] = mTotalStatistic[i];
        i++;
    }
    return i;
}

void ReliableModed::resetStatistic()
{
    mTotalStatistic.clear();
    mTotalStatistic.resize(getModelSize(), 0);
}
