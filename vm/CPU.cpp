#include "CPU.hpp"
#include <iostream>

using namespace std;

CPU::CPU() {}

void CPU::run()
{
    cout << "=== CPU started ===" << endl;
    for (int i = 0; i < theProgram.size; i++)
    {
        Instruction* inst = controlUnit.fetch(theProgram, i);
        controlUnit.decode(inst);
        controlUnit.execute(inst, arithmeticLogicUnit, registers);
        cout << endl;
    }
    cout << "=== CPU halted ===" << endl;
}

void CPU::printRegisters()
{
    cout << "=== Registers ==" << endl;
    for (int i = 0; i < 8; i++)
        cout << "  R" << i << " = " << registers.getRegister(i) << endl;
}

