//
// Created by oleksii on 1/19/23.
//

#include "CircuitModel.h"

void CircuitModel::setState(const std::vector<bool> &state, size_t modelSize)
{
    resetStateVecs();
    int i = setModulesFromState(modelSize, 0, state, ASkipped(), adapters, ACount());
    i = setModulesFromState(modelSize, i, state, BSkipped(), buses, BCount());
    i = setModulesFromState(modelSize, i, state, CSkipped(), controllers, CCount());
    i = setModulesFromState(modelSize, i, state, DSkipped(), detectors, DCount());
    i = setModulesFromState(modelSize, i, state, MSkipped(), mainlines, MCount());
    setModulesFromState(modelSize, i, state, PrSkipped(), processors, PrCount());
}

int CircuitModel::setModulesFromState(size_t modelSize, int iState,
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

size_t CircuitModel::getModelSize() const
{
    return ACount() + BCount() + CCount() + MCount() + PrCount() + DCount()
           - (ASkipped().size() + BSkipped().size() + CSkipped().size() + DSkipped().size()
              + MSkipped().size() + PrSkipped().size());
}

bool CircuitModel::A(size_t i)
{
    if (i > adapters.size()) {
        exit(1);
    }
    return adapters[i - 1];
}

bool CircuitModel::B(size_t i)
{
    if (i > buses.size()) {
        exit(1);
    }
    return buses[i - 1];
}

bool CircuitModel::C(size_t i)
{
    if (i > controllers.size()) {
        exit(1);
    }
    return controllers[i - 1];
}

bool CircuitModel::D(size_t i)
{
    if (i > detectors.size()) {
        exit(1);
    }
    return detectors[i - 1];
}

bool CircuitModel::M(size_t i)
{
    if (i > mainlines.size()) {
        exit(1);
    }
    return mainlines[i - 1];
}

bool CircuitModel::Pr(size_t i)
{
    if (i > processors.size()) {
        exit(1);
    }
    return processors[i - 1];
}

void CircuitModel::resetStateVecs()
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

double CircuitModel::getProbability()
{
    double res = 1;

    res *= getModuleReliabilityState(ACount(), ASkipped(), adapters, adaptersReliability());
    res *= getModuleReliabilityState(BCount(), BSkipped(), buses, busesReliability());
    res *= getModuleReliabilityState(CCount(), CSkipped(), controllers, controllersReliability());
    res *= getModuleReliabilityState(DCount(), DSkipped(), detectors, detectorsReliability());
    res *= getModuleReliabilityState(MCount(), MSkipped(), mainlines, mainlinesReliability());
    res *= getModuleReliabilityState(PrCount(), PrSkipped(), processors, processorsReliability());

    return res;
}

double CircuitModel::getModuleReliabilityState(size_t count, const std::vector<size_t> &skipped,
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
