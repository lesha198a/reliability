
#include "Redistribution.h"

#include <utility>
#include <iostream>
#include <sstream>

void Redistribution::setProcessorParams(const size_t procNum, const size_t nominalLoad,
                                        const size_t maxLoad)
{
    mMaxLoads[procNum - 1] = maxLoad;
    mNominalLoads[procNum - 1] = nominalLoad;
    if (std::ranges::find(mSkippedModules, procNum) == mSkippedModules.end()) {
        mTotalLoad += nominalLoad;
    }
}

void Redistribution::setProcessorsCount(const size_t count, std::vector<size_t> skippedModules)
{
    mCount = count;
    mSkippedModules = std::move(skippedModules);
    mMaxLoads.clear();
    mMaxLoads.resize(mCount, 0);
    mNominalLoads.clear();
    mNominalLoads.resize(mCount, 0);
    mTotalLoad = 0;
}

bool Redistribution::redistribute(std::vector<bool> state)
{
    mStates = std::move(state);
    mCurrentLoad = mNominalLoads;

    for (int i = 0; i < mStates.size(); ++i) {
        if (std::ranges::find(mSkippedModules, i + 1) != mSkippedModules.end()) {
            continue;
        }
        if (!mStates[i] && !redistributeModule(i)) {
            return false;
        }
    }
    if (!checkRedistribution()) {
        return false;
    }
    return true;
}

bool Redistribution::redistributeModule(int i)
{
    int j = -1;
    while (mCurrentLoad[i] > 0) {
        j++;
        if (std::ranges::find(mSkippedModules, j + 1) != mSkippedModules.end()) {
            continue;
        }
        if (j == i) {
            continue;
        }
        if (j >= mCount) {
            return false;
        }
        if (!mStates[j]) {
            continue;
        }

        const int available = (int)mMaxLoads[j] - (int)mCurrentLoad[j];
        if (available < 0) {
            return false;
        }
        const size_t toRedistribute = std::min<size_t>(available, mCurrentLoad[i]);
        mCurrentLoad[i] -= toRedistribute;
        mCurrentLoad[j] += toRedistribute;
    }
    return true;
}

bool Redistribution::checkRedistribution()
{
    size_t currentTotalLoad = 0;
    for (int i = 0; i < mStates.size(); ++i) {
        if (std::ranges::find(mSkippedModules, i + 1) != mSkippedModules.end()) {
            continue;
        }
        if (mStates[i]) {
            if (mCurrentLoad[i] > mMaxLoads[i]) {
                std::cout << "error: working processor has current load > max load" << std::endl;
                return false;
            }
            currentTotalLoad += mCurrentLoad[i];
        } else if (mCurrentLoad[i] != 0) {
            std::cout << "error: failed processor has current load != 0" << std::endl;
            return false;
        }
    }
    return currentTotalLoad == mTotalLoad;
}

std::string Redistribution::printTable()
{
    std::stringstream stream;
    stream << "N\t"
           << "Tn\t"
           << "Tm\t";
    for (int i = 0; i < mCount; ++i) {
        if (std::ranges::find(mSkippedModules, i + 1) == mSkippedModules.end()) {
            stream << "Pr" << i + 1 << "\t";
        }
    }
    stream << std::endl;
    for (int i = 0; i < mCount; ++i) {
        if (std::ranges::find(mSkippedModules, i + 1) != mSkippedModules.end()) {
            continue;
        }
        stream << "Pr" << i + 1 << "\t" << mNominalLoads[i] << "\t" << mMaxLoads[i] << "\t";
        for (int j = 0; j < mCount; ++j) {
            if (j == i) {
                stream << "-\t";
                continue;
            }
            if (std::ranges::find(mSkippedModules, j + 1) != mSkippedModules.end()) {
                continue;
            }
            int available = mMaxLoads[j] - mNominalLoads[j];

            stream << std::min(available, (int)mNominalLoads[i]) << "\t";
        }
        stream << std::endl;
    }
    return stream.str();
}
