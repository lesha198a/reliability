
#include <ctime>
#include <algorithm>
#include <set>
#include <iostream>
#include <sstream>
#include <random>
#include "Reliable.h"

double Reliable::calculateReliability(size_t failures, double percentage, bool redistribution)
{
    double result = 0;
    auto modelSize = mCircuitModel->getModelSize();
    size_t vecCount = factorialTriple(modelSize, failures, modelSize - failures, percentage);
    auto failurePosVecs = getFailurePositions((size_t)vecCount, failures, modelSize);

    for (const auto &failurePoses : failurePosVecs) {
        auto stateVec = mCircuitModel->generateState(failurePoses);
        mCircuitModel->setState(stateVec, modelSize);
        bool sysState = mCircuitModel->mainFunc();
        if (redistribution && !sysState) {
            bool redistributed = mRedistribution.redistribute(mCircuitModel->getProcessors());
            mRedistribStat++;
            if (redistributed) {
                mCompleteRedistribStat++;
                auto stateCopy = stateVec;
                mCircuitModel->setPrInStateVecTrue(stateCopy);
                mCircuitModel->setState(stateCopy, modelSize);
                sysState = mCircuitModel->mainFunc();
            }
        }
        if (sysState) {
            mCircuitModel->setState(stateVec, modelSize);
            result += mCircuitModel->getProbability();
        } else {
            appendStageFailureStatistic(stateVec);
        }
    }
    return result / percentage;
}

double Reliable::getProbabilityOfSuccesfulState()
{
    auto modelSize = mCircuitModel->getModelSize();
    auto stateVec = mCircuitModel->generateState({});
    mCircuitModel->setState(stateVec, modelSize);
    double reliabilityOfTrueState = mCircuitModel->getProbability();
    return reliabilityOfTrueState;
}

size_t Reliable::factorialTriple(size_t dividend, size_t divisor, size_t divisor2, double resScale)
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

std::mt19937 generator(std::random_device{}());

std::vector<std::set<size_t>> Reliable::getFailurePositions(size_t vecCount, size_t failures,
                                                            size_t modelSize) const
{
    std::vector<std::set<size_t>> res;
    for (int i = 0; i < vecCount; ++i) {
        std::set<size_t> positions;
        while (positions.size() <= failures) {
            size_t pos = generator() % modelSize;
            positions.insert(pos);
        }
        res.push_back(positions);
    }
    return res;
}

void Reliable::appendStageFailureStatistic(std::vector<bool> state)
{
    for (int i = 0; i < state.size(); ++i) {
        if (!state[i]) {
            mTotalStatistic[i]++;
        }
    }
}

std::map<std::string, size_t> Reliable::getStatistics()
{
    return mCircuitModel->getStatistics(mTotalStatistic);
}

void Reliable::resetStatistic()
{
    mRedistribStat = 0;
    mCompleteRedistribStat = 0;
    mTotalStatistic.clear();
    mTotalStatistic.resize(mCircuitModel->getModelSize(), 0);
}

void Reliable::amendStatistics(std::map<std::string, size_t> &mainStat,
                               const std::map<std::string, size_t> &otherStat)
{
    for (const auto &item : otherStat) {
        mainStat[item.first] = item.second;
    }
}

Reliable::Reliable(std::unique_ptr<CircuitModel> model) : mCircuitModel(std::move(model))
{
    mCircuitModel->setRedistributionTable(mRedistribution);
    resetStatistic();
}
