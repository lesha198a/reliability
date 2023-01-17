//
// Created by lesha on 14.01.2023.
//

#include <ctime>
#include <algorithm>
#include <set>
#include <iostream>
#include <sstream>
#include "Reliable.h"

Reliable::Reliable()
{
    mRedistribution.setProcessorsCount(PrCount(), PrSkipped());
    mRedistribution.setProcessorParams(1, 50, 100);
    mRedistribution.setProcessorParams(2, 60, 90);
    mRedistribution.setProcessorParams(3, 40, 90);
    mRedistribution.setProcessorParams(4, 70, 100);
    mRedistribution.setProcessorParams(5, 20, 40);
    mRedistribution.setProcessorParams(6, 40, 50);
}

bool Reliable::A(size_t i)
{
    if (i > adapters.size()) {
        exit(1);
    }
    return adapters[i - 1];
}

bool Reliable::B(size_t i)
{
    if (i > buses.size()) {
        exit(1);
    }
    return buses[i - 1];
}

bool Reliable::C(size_t i)
{
    if (i > controllers.size()) {
        exit(1);
    }
    return controllers[i - 1];
}

bool Reliable::M(size_t i)
{
    if (i > mainlines.size()) {
        exit(1);
    }
    return mainlines[i - 1];
}

bool Reliable::Pr(size_t i)
{
    if (i > processors.size()) {
        exit(1);
    }
    return processors[i - 1];
}

bool Reliable::D(size_t i)
{
    if (i > detectors.size()) {
        exit(1);
    }
    return detectors[i - 1];
}

double Reliable::calculateReliability(size_t failures, double percentage, bool redistribution)
{
    double result = 0;
    auto modelSize = getModelSize();
    size_t vecCount = factorialTriple(modelSize, failures,
                                       modelSize - failures,percentage);
    auto failurePosVecs = getFailurePositions((size_t )vecCount, failures, modelSize);

    resetStageStatistic(modelSize);
    for (const auto &failurePoses : failurePosVecs) {
        auto stateVec = getStateFrom(failurePoses, modelSize);
        setState(stateVec, modelSize);
        bool sysState = mainFunc()
                        || (redistribution && mRedistribution.redistribute(getPrState(stateVec)));
        if (sysState) {
            result += getProbability();
        } else {
            appendStageFailureStatistic(stateVec);
        }
    }
//    std::cout << "Result: " << result / percentage << std::endl;
    finishStageStatistic();
    return result / percentage;
}

size_t Reliable::getModelSize() const
{
    return ACount() + BCount() + CCount() + MCount() + PrCount() + DCount()
           - (ASkipped().size() + BSkipped().size() + CSkipped().size() + DSkipped().size()
              + MSkipped().size() + PrSkipped().size() + DSkipped().size());
}

size_t Reliable::factorialTriple(size_t dividend, size_t divisor, size_t divisor2,
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

std::vector<std::set<size_t>> Reliable::getFailurePositions(size_t vecCount, size_t failures,
                                                            size_t modelSize) const
{
    std::vector<std::set<size_t>> res;
    for (int i = 0; i < vecCount; ++i) {
        std::set<size_t> positions;
        while (positions.size() <= failures) {
            positions.insert(std::rand() % modelSize);
        }
        res.push_back(positions);
    }
    return res;
}

std::vector<bool> Reliable::getStateFrom(const std::set<size_t> &failurePoses, size_t modelSize)
{
    std::vector<bool> res;
    res.resize(modelSize, true);
    for (const auto &failurePos : failurePoses) {
        res[failurePos] = false;
    }
    return res;
}

void Reliable::setState(const std::vector<bool> &state, size_t modelSize)
{
    resetStateVecs();
    int i = setModulesFromState(modelSize, 0, state, ASkipped(), adapters, ACount());
    i = setModulesFromState(modelSize, i, state, BSkipped(), buses, BCount());
    i = setModulesFromState(modelSize, i, state, CSkipped(), controllers, CCount());
    i = setModulesFromState(modelSize, i, state, DSkipped(), detectors, DCount());
    i = setModulesFromState(modelSize, i, state, MSkipped(), mainlines, MCount());
    setModulesFromState(modelSize, i, state, PrSkipped(), processors, PrCount());
}

int Reliable::setModulesFromState(size_t modelSize, int iState,
                                  const std::vector<bool> &localState,
                                  const std::vector<size_t> &skipped, std::vector<bool> &vecToSet,
                                  size_t count) const
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

double Reliable::getProbability()
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

double Reliable::getModuleReliabilityState(size_t count, const std::vector<size_t> &skipped,
                                           const std::vector<bool> &vec, double reliability) const
{
    double res = 1;
    for (int i = 0; i < count; ++i) {
        if (std::find(skipped.begin(), skipped.end(), i + 1) != skipped.end()) {
            continue;
        }
        if (vec[i]) {
            res *= 1 - reliability;
        } else {
            res *= reliability;
        }
    }
    return res;
}

void Reliable::appendStageFailureStatistic(std::vector<bool> state) {
    for (int i = 0; i < state.size(); ++i) {
        if (!state[i]) {
            mStageStatistic[i]++;
        }
    }
}

void Reliable::finishStageStatistic() {
    for (int i = 0; i < mTotalStatistic.size(); ++i) {
        mTotalStatistic[i] += mStageStatistic[i];
    }
}

void Reliable::resetStageStatistic(size_t modelSize) {
    mStageStatistic.clear();
    mStageStatistic.resize(modelSize, 0);
}

std::vector<bool> Reliable::getPrState(const std::vector<bool> &fullState)
{
    std::vector<bool> res;
    res.resize(PrCount(), false);
    int padding = (int)fullState.size() - (int)PrCount();
    for (int i = 0; i < PrCount(); ++i) {
        res[i] = fullState[padding + i];
    }
    return res;
}

std::map<std::string, size_t> Reliable::getStatistics()
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

int Reliable::statisticForModule(size_t count, const std::vector<size_t> &skipped,
                                 std::map<std::string, size_t> &res, int i,
                                 const std::string &name) const
{
    std::stringstream stream;
    for (int j = 0; j < mTotalStatistic.size() && j < count; ++j) {
        if (std::find(skipped.begin(), skipped.end(), j + i + 1) != skipped.end()) {
            continue;
        }
        stream << name << j;
        auto resName = stream.str();
        stream.clear();
        res[resName] = mTotalStatistic[i + j];
    }
    return i + (int)count;
}
