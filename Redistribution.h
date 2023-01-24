
#ifndef PROGRAM_REDISTRIBUTION_H
#define PROGRAM_REDISTRIBUTION_H

#include <vector>

class Redistribution
{
public:
    Redistribution() = default;
    void setProcessorsCount(size_t count, std::vector<size_t> skippedModules);
    void setProcessorParams(size_t procNum, size_t nominalLoad, size_t maxLoad);

    bool redistribute(std::vector<bool> state);

    void printTable();

private:
    bool redistributeModule(int i);
    bool checkRedistribution();

    std::vector<bool> mStates;
    std::vector<size_t> mCurrentLoad;

    size_t mCount = 0;
    std::vector<size_t> mSkippedModules;
    std::vector<size_t> mNominalLoads;
    std::vector<size_t> mMaxLoads;
    size_t mTotalLoad = 0;
};

#endif //PROGRAM_REDISTRIBUTION_H
