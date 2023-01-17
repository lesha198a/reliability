//
// Created by oleksii on 1/16/23.
//

#include "Redistribution.h"

#include <utility>
#include <iostream>

void Redistribution::setProcessorParams(size_t procNum, size_t nominalLoad, size_t maxLoad)
{
    mMaxLoads[procNum - 1] = maxLoad;
    mNominalLoads[procNum - 1] = nominalLoad;
    mTotalLoad = 0;
    for (const auto &moduleLoad : mNominalLoads) {
        mTotalLoad += moduleLoad;
    }
}

void Redistribution::setProcessorsCount(size_t count, std::vector<size_t> skippedModules)
{
    mCount = count;
    mSkippedModules = std::move(skippedModules);
    mMaxLoads.clear();
    mMaxLoads.resize(mCount, 0);
    mNominalLoads.clear();
    mNominalLoads.resize(mCount, 0);
}

bool Redistribution::redistribute(std::vector<bool> state)
{
    mStates = std::move(state);
    mCurrentLoad = mNominalLoads;

    for (int i = 0; i < mStates.size(); ++i) {
        if (std::find(mSkippedModules.begin(), mSkippedModules.end(), i) != mSkippedModules.end()) {
            continue;
        }
        if (!mStates[i] && !redistributeModule(i)) {
            return false;
        }
    }

    return checkRedistribution();
}

bool Redistribution::redistributeModule(int i) {
    int j = -1;
    while (mCurrentLoad[i] > 0) {
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

        int available = (int)mMaxLoads[j] - (int)mCurrentLoad[j];
        if (available < 0) {
            return false;
        }
        size_t toRedistribute = std::min<size_t>(available, mCurrentLoad[i]);
        mCurrentLoad[i] -= toRedistribute;
        mCurrentLoad[j] += toRedistribute;
    }
    return true;
}

bool Redistribution::checkRedistribution()
{
    size_t currentTotalLoad = 0;
    for (int i = 0; i < mStates.size(); ++i) {
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
