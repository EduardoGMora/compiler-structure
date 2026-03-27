#ifndef ADD_HPP
#define ADD_HPP

#include "Instruction.hpp"
#include "Registers.hpp"
#include "ALU.hpp"

// ADD destReg, srcReg  →  destReg = destReg + srcReg
class ADD : public Instruction
{
private:
    int destReg;
    int srcReg;

public:
    ADD(std::string name, int code, int length, int destReg, int srcReg);
    void execute(Registers& reg, ALU& alu) override;
};

#endif
