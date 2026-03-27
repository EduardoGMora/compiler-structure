#ifndef CPU_HPP
#define CPU_HPP

#include "ControlUnit.hpp"
#include "ALU.hpp"
#include "Program.hpp"
#include "Registers.hpp"

#include <string>

class CPU
{
    public:
        ControlUnit controlUnit;
        ALU arithmeticLogicUnit;
        Registers registers;
        std::string status;
        Program theProgram;

        CPU();

        void run();
        void printRegisters();
};

#endif