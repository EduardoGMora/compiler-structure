#ifndef REGISTERS_HPP
#define REGISTERS_HPP

class Registers
{
    public:
        int R[16]; // 16 general-purpose registers
        int PC;    // Program Counter
        int SP;    // Stack Pointer
        int FP;    // Frame Pointer
    
        Registers();
        int getRegister(int index);
        void setRegister(int index, int value);
};

#endif