//
// Created by oleksii on 1/16/23.
//

#include "Redistribution.h"

#include <utility>

void Redistribution::setProcessorParams(size_t procNum, size_t nominalLoad, size_t maxLoad,
                                        std::vector<size_t> redistributionOptions)
{
    mMaxLoads[procNum] = maxLoad;
    mNominalLoads[procNum] = nominalLoad;
    mRedistribution[procNum] = std::move(redistributionOptions);
}

void Redistribution::setProcessorsCount(size_t count, std::vector<size_t> skippedModules)
{
    mCount = count;
    mSkippedModules = std::move(skippedModules);
    mMaxLoads.clear();
    mMaxLoads.resize(mCount, 0);
    mNominalLoads.clear();
    mNominalLoads.resize(mCount, 0);
    mRedistribution.clear();
    std::vector<size_t> redistributionForOneModule;
    redistributionForOneModule.resize(mCount, 0);
    mRedistribution.resize(mCount, redistributionForOneModule);
}

bool Redistribution::redistribute(std::vector<bool> state)
{
    mStates = std::move(state);
    mRedistributed.clear();
    mRedistributed.resize(mCount, false);
    mCurrentLoad = mNominalLoads;

    for (int i = 0; i < mStates.size(); ++i) {
        if (std::find(mSkippedModules.begin(), mSkippedModules.end(), i) != mSkippedModules.end()) {
            continue;
        }
        if (!mStates[i] && !redistributeModule(i)) {
            return false;
        }
    }



    return true;
}

bool Redistribution::redistributeModule(int i) {
    int j = -1;
    while (mCurrentLoad[i] != 0) {
        j++;
        if (j == i) {
            continue;
        }
        if (j >= mCount) {
            return false;
        }
        if (!mStates[j]) {
            continue;
        }

    }
    return false; }
