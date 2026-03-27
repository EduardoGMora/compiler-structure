#include "ALU.hpp"
#include <stdexcept>

ALU::ALU() {}

int ALU::add(int a, int b)      { return a + b; }
int ALU::subtract(int a, int b) { return a - b; }
int ALU::multiply(int a, int b) { return a * b; }
int ALU::divide(int a, int b)
{
    if (b == 0)
        throw std::runtime_error("Division by zero");
    return a / b;
}