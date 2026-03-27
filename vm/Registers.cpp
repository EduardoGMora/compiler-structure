#include "Registers.hpp"

Registers::Registers() : PC(0), SP(0), FP(0)
{
    for (int i = 0; i < 16; i++)
        R[i] = 0;
}

int Registers::getRegister(int index)
{
    return R[index];
}

void Registers::setRegister(int index, int value)
{
    R[index] = value;
}
