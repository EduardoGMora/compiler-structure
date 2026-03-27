#ifndef SUB_HPP
#define SUB_HPP

#include "Instruction.hpp"
#include "Registers.hpp"
#include "ALU.hpp"

// SUB destReg, srcReg  →  destReg = destReg - srcReg
class SUB : public Instruction
{
private:
    int destReg;
    int srcReg;

public:
    SUB(std::string name, int code, int length, int destReg, int srcReg);
    void execute(Registers& reg, ALU& alu) override;
};

#endif
