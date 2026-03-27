#include "MOV.hpp"
#include <iostream>

using namespace std;

MOV::MOV(string name, int code, int length, int operand, int registerIndex)
    : Instruction(name, code, length), operand(operand), registerIndex(registerIndex) {}

int MOV::getOperand() {
    return operand;
}

int MOV::getRegisterIndex() {
    return registerIndex;
}

void MOV::setRegisterIndex(int index) {
    registerIndex = index;
}

void MOV::execute(Registers& reg, ALU& alu)
{
    int registerIndex = getRegisterIndex();  // R0
    reg.setRegister(registerIndex, operand);
    cout << "  R" << registerIndex << " <- " << operand << endl;
}