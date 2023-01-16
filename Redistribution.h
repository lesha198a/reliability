//
// Created by oleksii on 1/16/23.
//

#ifndef PROGRAM_REDISTRIBUTION_H
#define PROGRAM_REDISTRIBUTION_H

#include <vector>

class Redistribution
{
public:
    Redistribution();
    void setProcessorsCount(size_t count, std::vector<size_t> skippedModules);
    void setProcessorParams(size_t procNum, size_t nominalLoad, size_t maxLoad,
                            std::vector<size_t> redistributionOptions);

    bool redistribute(std::vector<bool> state);

private:
    std::vector<bool> mStates;
    std::vector<bool> mRedistributed;
    std::vector<size_t> mCurrentLoad;

    size_t mCount = 0;
    std::vector<size_t> mSkippedModules;
    std::vector<size_t> mNominalLoads;
    std::vector<size_t> mMaxLoads;
    std::vector<std::vector<size_t>> mRedistribution;
    bool redistributeModule(int i);
};

#endif //PROGRAM_REDISTRIBUTION_H
