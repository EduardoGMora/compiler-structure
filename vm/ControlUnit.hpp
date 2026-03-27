#ifndef CONTROLUNIT_HPP
#define CONTROLUNIT_HPP

#include <string>
#include <iostream>
#include "Instruction.hpp"
#include "ALU.hpp"
#include "Program.hpp"
#include "Registers.hpp"

class ControlUnit
{
    public:
		ControlUnit();
		Instruction* fetch(Program& theProgram, int thePosition);
		int decode(Instruction* theInstruction);
		void execute(Instruction* inst, ALU& alu, Registers& reg);
};

#endif