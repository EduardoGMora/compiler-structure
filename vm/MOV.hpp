#ifndef MOV_HPP
#define MOV_HPP

#include "Instruction.hpp"
#include "Registers.hpp"
#include "ALU.hpp"

class MOV : public Instruction
{
private:
    int operand;        // The value to load
    int registerIndex;  // Destination register (e.g. R0, R1...)

public:
    MOV(std::string name, int code, int length, int operand, int registerIndex = 0);
    int getOperand();
    int getRegisterIndex();
    void setRegisterIndex(int index);
    void execute(Registers& reg, ALU& alu) override;
};

#endif