#include <iostream>
#include "CPU.hpp"
#include "MOV.hpp"
#include "ADD.hpp"
#include "SUB.hpp"
#include "MUL.hpp"

using namespace std;

int main()
{
    CPU cpu;

    // Programa:
    //   R0 = 10
    //   R1 = 5
    //   R0 = R0 + R1   → R0 = 15
    //   R0 = R0 - R1   → R0 = 10
    //   R0 = R0 * R1   → R0 = 50

    cpu.theProgram.addInstruction(new MOV("MOV R0, 10", 1, 1, 10, 0), 0);
    cpu.theProgram.addInstruction(new MOV("MOV R1, 5",  1, 1,  5, 1), 1);
    cpu.theProgram.addInstruction(new ADD("ADD R0, R1", 2, 1, 0, 1), 2);
    cpu.theProgram.addInstruction(new SUB("SUB R0, R1", 3, 1, 0, 1), 3);
    cpu.theProgram.addInstruction(new MUL("MUL R2, R1", 4, 1, 0, 1), 4);

    cpu.run();
    cpu.printRegisters();

    return 0;
}
