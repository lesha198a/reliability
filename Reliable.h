//
// Created by lesha on 14.01.2023.
//

#ifndef PROGRAM_RELIABLE_H
#define PROGRAM_RELIABLE_H

#include <vector>

class Reliable
{
public:
    Reliable() = default;

private:
    //count with skipped numbers of elements on circuit (aka put max number from circuit)
    static size_t ACount() { return 3; }
    static size_t BCount() { return 4; }
    static size_t CCount() { return 6; }
    static size_t MCount() { return 2; }
    static size_t PrCount() { return 6; }
    static size_t DCount() { return 8; }

    std::vector<size_t> ASkipped() { return {2}; }
    std::vector<size_t> BSkipped() { return {}; }
    std::vector<size_t> CSkipped() { return {3}; }
    std::vector<size_t> MSkipped() { return {}; }
    std::vector<size_t> PrSkipped() { return {}; }
    std::vector<size_t> DSkipped() { return {4}; }

    bool func1()
    {
        return D(2) and (C(1) or C(2)) and (B(1) or B(2)) and (Pr(1) or Pr(2)) and A(1) and M(1);
    }

    bool func2()
    {
        return D(3) and C(2) and (B(1) or B(2))
               and (Pr(3) or (A(1) and M(1) and C(4) and (D(5) or D(6))));
    }

    bool func3()
    {
        return D(1) and C(1) and (B(1) or B(2))
               and (Pr(4) or (A(1) and M(2) and A(3) and B(4) and C(6) and D(8)));
    }

    bool func4()
    {
        return (D(7) or D(8)) and C(5) and B(3) and (Pr(5) or Pr(6)) and B(4) and A(3) and M(2);
    }

    bool mainFunc() { return func1() and func2() and func3() and func4(); }

    void resetStateVecs() {

    }

    //indexation starts from 1
    bool A(size_t i);
    bool B(size_t i);
    bool C(size_t i);
    bool M(size_t i);
    bool Pr(size_t i);
    bool D(size_t i);

    std::vector<bool> adapters;    //A
    std::vector<bool> buses;       //B
    std::vector<bool> controllers; //C
    std::vector<bool> networks;    //M
    std::vector<bool> processors;  //Pr
    std::vector<bool> sensors;     //D
};

#endif //PROGRAM_RELIABLE_H
