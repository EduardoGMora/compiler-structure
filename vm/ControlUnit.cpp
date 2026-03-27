#include "ControlUnit.hpp"
#include <iostream>

using namespace std;

ControlUnit::ControlUnit() {}

Instruction* ControlUnit::fetch(Program& theProgram, int thePosition)
{
    cout << "[FETCH]   position " << thePosition << endl;
    return theProgram.getInstruction(thePosition);
}

int ControlUnit::decode(Instruction* theInstruction)
{
    cout << "[DECODE]  " << theInstruction->getName() << " (code=" << theInstruction->getCode() << ")" << endl;
    return theInstruction->getCode();
}

void ControlUnit::execute(Instruction* inst, ALU& alu, Registers& reg)
{
    cout << "[EXECUTE] " << inst->getName() << endl;
    inst->execute(reg, alu);  // polimorfismo: llama al execute de MOV, ADD, SUB, MUL...
}

