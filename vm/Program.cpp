#include "Program.hpp"

Program::Program() : size(0)
{
    for (int i = 0; i < 10; i++)
        instructions[i] = nullptr;
}

void Program::addInstruction(Instruction* theInstruction, int thePosition)
{
	instructions[thePosition] = theInstruction;
	if (thePosition >= size)
		size = thePosition + 1;
}

Instruction* Program::getInstruction(int thePosition)
{
	return instructions[thePosition];
}