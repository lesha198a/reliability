//
// Created by lesha on 14.01.2023.
//

#include <ctime>
#include <algorithm>
#include <set>
#include <iostream>
#include "Reliable.h"

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

void Reliable::calculate()
{
    std::srand(std::time(nullptr));
    double result = 0;
    auto modelSize = getModelSize();
    size_t failures = 1;
    double percentage = 1;
    int64_t vecCount = factorialTriple(factorialTriple(modelSize, failures, 0, 0),
                                       modelSize - failures, 0, 0);
    int vecCountCropped = vecCount * percentage;
    auto failurePosVecs = getFailurePositions(vecCountCropped, failures, modelSize);
    for (const auto &failurePoses : failurePosVecs) {
        auto stateVec = getStateFrom(failurePoses, modelSize);
        setState(stateVec, modelSize);
        bool sysState = mainFunc(); //todo redistrib
        if (sysState) {
            result += getProbability();
            std::cout << "prob " << result <<std::endl;
        }
    }
    std::cout << "Result: " << result / percentage << std::endl;
}

size_t Reliable::getModelSize() const
{
    return ACount() + BCount() + CCount() + MCount() + PrCount() + DCount()
           - (ASkipped().size() + BSkipped().size() + CSkipped().size() + DSkipped().size()
              + MSkipped().size() + PrSkipped().size() + DSkipped().size());
}

int64_t Reliable::factorialTriple(int64_t dividend, int64_t divisor, int64_t divisor2,
                                  double resScale)
{
    int64_t dividendFixed = dividend;
    if (dividend < 2) {
        dividendFixed = 1;
    }
    int64_t divisorFixed = divisor;
    if (divisor < 2) {
        divisorFixed = 1;
    }
    int64_t divisor2Fixed = divisor2;
    if (divisor2 < 2) {
        divisor2Fixed = 1;
    }
    int64_t dividendLeft = 1;
    int64_t dividendRight = 1;
    int64_t divisorLeft = 1;
    int64_t divisorRight = 1;
    if (divisorFixed < dividendFixed) {
        dividendLeft = divisorFixed + 1;
        dividendRight = dividendFixed;
    } else {
        divisorLeft = dividendFixed + 1;
        divisorRight = divisorFixed;
    }
    int64_t divisor2Left1 = 1;
    int64_t divisor2Right1 = 1;
    int64_t divisor2Left2 = 1;
    int64_t divisor2Right2 = 1;
    if (divisor2Fixed >= dividendLeft) {
        divisor2Right1 = dividendLeft - 1;
        dividendLeft = divisor2Fixed + 1;
        if (divisor2Fixed < dividendRight) {
            dividendLeft = divisor2Fixed + 1;
        }

    }


    int64_t res = 1;
    for (int64_t i = min + 1; i < max + 1; i++) {
        res *= i;
    }
    return res;
}

std::vector<std::set<size_t>> Reliable::getFailurePositions(size_t vecCount, size_t failures,
                                                            size_t modelSize) const
{
    std::vector<std::set<size_t>> res;
    for (int i = 0; i < vecCount; ++i) {
        std::vector<size_t> positions;
        while (positions.size() < failures) {
            positions.push_back(std::rand() % modelSize);
        }
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
