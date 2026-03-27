#include "MUL.hpp"
#include <iostream>

using namespace std;

MUL::MUL(string name, int code, int length, int destReg, int srcReg)
    : Instruction(name, code, length), destReg(destReg), srcReg(srcReg) {}

void MUL::execute(Registers& reg, ALU& alu)
{
    int result = alu.multiply(reg.getRegister(destReg), reg.getRegister(srcReg));
    reg.setRegister(destReg, result);
    cout << "  R" << destReg << " <- R" << destReg << " * R" << srcReg << " = " << result << endl;
}
