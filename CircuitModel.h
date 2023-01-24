//
// Created by oleksii on 1/19/23.
//

#ifndef PROGRAM_CIRCUITMODEL_H
#define PROGRAM_CIRCUITMODEL_H

#include <vector>

class CircuitModel
{
public:
    void setState(const std::vector<bool> &state, size_t modelSize);
    double getProbability();

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

    size_t getModelSize() const;

    bool mainFunc() { return func1() and func2() and func3() and func4(); }

protected:
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
    size_t ACount() const override { return 4; }
    size_t BCount() const override { return 5; }
    size_t CCount() const override { return 6; }
    size_t DCount() const override { return 8; }
    size_t MCount() const override { return 3; }
    size_t PrCount() const override { return 6; }
    std::vector<size_t> ASkipped() const override { return {}; }
    std::vector<size_t> BSkipped() const override { return {}; }
    std::vector<size_t> CSkipped() const override { return {}; }
    std::vector<size_t> DSkipped() const override { return {4}; }
    std::vector<size_t> MSkipped() const override { return {}; }
    std::vector<size_t> PrSkipped() const override { return {}; }

protected:
    bool func1() override
    {
        return (D(1) or D(2) or D(3)) and (C(1) or C(2) or C(3)) and (B(1) or B(2))
               and (Pr(1) or Pr(2) or Pr(3) or Pr(4)) and (A(1) or A(2));
    }
    bool func2() override
    {
        return (A(1) or A(2)) and (M(1) or M(2))
               and (C(4) and (D(5) or D(6)) or A(3) and (B(4) or B(5)));
    }
    bool func3() override
    {
        return (A(4) and (B(3) or B(5) or B(5) and A(3) and B(4)) or A(3) and (B(4) or B(5)))
               and (Pr(5) or Pr(6) or (C(5) or C(6)) and (D(7) or D(8)));
    }
    bool func4() override { return (A(1) or A(2)) and (M(3) or M(1)); }

    double adaptersReliability() override { return 4.2e-4; }    //A
    double busesReliability() override { return 1.4e-5; }       //B
    double controllersReliability() override { return 3.1e-4; } //C
    double detectorsReliability() override { return 2.2e-5; }   //D
    double mainlinesReliability() override { return 1.3e-4; }   //M
    double processorsReliability() override { return 6.2e-4; }  //Pr
};

#endif //PROGRAM_CIRCUITMODEL_H
