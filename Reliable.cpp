//
// Created by lesha on 14.01.2023.
//

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
