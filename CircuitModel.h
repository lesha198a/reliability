//
// Created by oleksii on 1/19/23.
//

#ifndef PROGRAM_CIRCUITMODEL_H
#define PROGRAM_CIRCUITMODEL_H

#include <vector>
#include <map>
#include <string>
#include <set>
#include "Redistribution.h"

class CircuitModel
{
public:
    virtual ~CircuitModel() = default;
    void setState(const std::vector<bool> &state, size_t modelSize);
    double getProbability();
    std::map<std::string, size_t> getStatistics(const std::vector<size_t> &statistic);

    size_t getModelSize() const;

    bool mainFunc() { return func1() and func2() and func3() and func4(); }

    void setPrInStateVecTrue(std::vector<bool> &state) const;
    std::vector<bool> getProcessors() { return processors; }
    void setRedistributionTable(Redistribution &redistribution) const;
    std::vector<bool> generateState(const std::set<size_t> &failurePoses) const;

protected:
    //count with skipped numbers of elements on circuit (aka put max number from circuit)
    virtual size_t ACount() const = 0;
    virtual size_t BCount() const = 0;
    virtual size_t CCount() const = 0;
    virtual size_t DCount() const = 0;
    virtual size_t MCount() const = 0;
    virtual size_t PrCount() const = 0;

    virtual std::vector<size_t> ASkipped() const = 0;
    virtual std::vector<size_t> BSkipped() const = 0;
    virtual std::vector<size_t> CSkipped() const = 0;
    virtual std::vector<size_t> DSkipped() const = 0;
    virtual std::vector<size_t> MSkipped() const = 0;
    virtual std::vector<size_t> PrSkipped() const = 0;

    //indexation starts from 1
    bool A(size_t i);
    bool B(size_t i);
    bool C(size_t i);
    bool D(size_t i);
    bool M(size_t i);
    bool Pr(size_t i);

    virtual bool func1() = 0;

    virtual bool func2() = 0;

    virtual bool func3() = 0;

    virtual bool func4() = 0;

    virtual double adaptersReliability() = 0;
    virtual double busesReliability() = 0;
    virtual double controllersReliability() = 0;
    virtual double detectorsReliability() = 0;
    virtual double mainlinesReliability() = 0;
    virtual double processorsReliability() = 0;

private:
    void resetStateVecs();
    int setModulesFromState(size_t modelSize, int iState, const std::vector<bool> &localState,
                            const std::vector<size_t> &skipped, std::vector<bool> &vecToSet,
                            size_t count) const;
    double getModuleReliabilityState(size_t count, const std::vector<size_t> &skipped,
                                     const std::vector<bool> &vec, double reliability) const;
    int statisticForModule(size_t count, const std::vector<size_t> &skipped,
                           std::map<std::string, size_t> &res, int i, const std::string &name,
                           const std::vector<size_t> &statistic) const;

    std::vector<bool> adapters;    //A
    std::vector<bool> buses;       //B
    std::vector<bool> controllers; //C
    std::vector<bool> detectors;   //D
    std::vector<bool> mainlines;   //M
    std::vector<bool> processors;  //Pr
};

class OrigCircuit : public CircuitModel
{
public:
    size_t ACount() const override { return 2; }
    size_t BCount() const override { return 5; }
    size_t CCount() const override { return 6; }
    size_t DCount() const override { return 8; }
    size_t MCount() const override { return 2; }
    size_t PrCount() const override { return 6; }
    std::vector<size_t> ASkipped() const override { return {}; }
    std::vector<size_t> BSkipped() const override { return {3}; }
    std::vector<size_t> CSkipped() const override { return {3}; }
    std::vector<size_t> DSkipped() const override { return {4, 5, 7}; }
    std::vector<size_t> MSkipped() const override { return {}; }
    std::vector<size_t> PrSkipped() const override { return {4}; }

protected:
    bool func1() override
    {
        return (C(1) and (D(1) or D(2)))
               or (C(2) and (D(2) or D(3))) and (B(1) or B(2))
                      and (Pr(1) or Pr(2) or Pr(3)
                           or (A(1) and (M(1) or M(2))));
    }
    bool func2() override { return D(8) and (C(5) or C(6)) and (B(4) or B(5)) and (A(2) and (M(1) and C(4) and D(6) or M(2)) or Pr(6)); }
    bool func3() override { return B(4) and (A(2) or Pr(5) or Pr(6) or C(5) or C(6)); }
    bool func4() override { return true; }

    double adaptersReliability() override { return 1.2e-4; }    //A
    double busesReliability() override { return 1.5e-5; }       //B
    double controllersReliability() override { return 4.1e-4; } //C
    double detectorsReliability() override { return 2.2e-5; }   //D
    double mainlinesReliability() override { return 3.6e-4; }   //M
    double processorsReliability() override { return 1.2e-4; }  //Pr
};

class ModifiedCircuit : public CircuitModel
{
public:
    size_t ACount() const override { return 2; }
    size_t BCount() const override { return 5; }
    size_t CCount() const override { return 6; }
    size_t DCount() const override { return 8; }
    size_t MCount() const override { return 2; }
    size_t PrCount() const override { return 6; }
    std::vector<size_t> ASkipped() const override { return {}; }
    std::vector<size_t> BSkipped() const override { return {3}; }
    std::vector<size_t> CSkipped() const override { return {3}; }
    std::vector<size_t> DSkipped() const override { return {4, 5}; }
    std::vector<size_t> MSkipped() const override { return {}; }
    std::vector<size_t> PrSkipped() const override { return {4}; }

protected:
    bool func1() override
    {
        return (C(1) and (D(1) or D(2)))
               or (C(2) and (D(2) or D(3))) and (B(1) or B(2))
                      and (Pr(1) or Pr(2) or Pr(3)
                           or (A(1) and (M(1) or M(2))));
    }
    bool func2() override { return (D(7) or D(8)) and (C(5) or C(6)) and (B(4) or B(5)) and (A(2) and (M(1) and C(4) and D(6) or M(2)) or Pr(6) or Pr(5)); }
    bool func3() override { return true; }
    bool func4() override { return true; }

    double adaptersReliability() override { return 1.2e-4; }    //A
    double busesReliability() override { return 1.5e-5; }       //B
    double controllersReliability() override { return 4.1e-4; } //C
    double detectorsReliability() override { return 2.2e-5; }   //D
    double mainlinesReliability() override { return 3.6e-4; }   //M
    double processorsReliability() override { return 1.2e-4; }  //Pr
};

#endif //PROGRAM_CIRCUITMODEL_H
