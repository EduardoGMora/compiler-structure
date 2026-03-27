#ifndef MUL_HPP
#define MUL_HPP

#include "Instruction.hpp"
#include "Registers.hpp"
#include "ALU.hpp"

// MUL destReg, srcReg  →  destReg = destReg * srcReg
class MUL : public Instruction
{
private:
    int destReg;
    int srcReg;

public:
    MUL(std::string name, int code, int length, int destReg, int srcReg);
    void execute(Registers& reg, ALU& alu) override;
};

#endif
