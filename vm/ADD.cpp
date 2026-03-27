#include "ADD.hpp"
#include <iostream>

using namespace std;

ADD::ADD(string name, int code, int length, int destReg, int srcReg)
    : Instruction(name, code, length), destReg(destReg), srcReg(srcReg) {}

void ADD::execute(Registers& reg, ALU& alu)
{
    int result = alu.add(reg.getRegister(destReg), reg.getRegister(srcReg));
    reg.setRegister(destReg, result);
    cout << "  R" << destReg << " <- R" << destReg << " + R" << srcReg << " = " << result << endl;
}
