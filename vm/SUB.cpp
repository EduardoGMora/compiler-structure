#include "SUB.hpp"
#include <iostream>

using namespace std;

SUB::SUB(string name, int code, int length, int destReg, int srcReg)
    : Instruction(name, code, length), destReg(destReg), srcReg(srcReg) {}

void SUB::execute(Registers& reg, ALU& alu)
{
    int result = alu.subtract(reg.getRegister(destReg), reg.getRegister(srcReg));
    reg.setRegister(destReg, result);
    cout << "  R" << destReg << " <- R" << destReg << " - R" << srcReg << " = " << result << endl;
}
