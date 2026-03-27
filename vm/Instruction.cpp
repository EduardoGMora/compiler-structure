#include "Instruction.hpp"
#include <iostream>

using namespace std;

Instruction::Instruction() {}

Instruction::Instruction(string theName, short int theCode, short int theLength) {
    name = theName;
    code = theCode;
    length = theLength;
}

void Instruction::display() {
    cout << "Instruction: " << name << ", Code: " << code << ", Length: " << length << endl;
}

int Instruction::getCode() {
    return code;
}

string Instruction::getName() {
    return name;
}

void Instruction::execute(Registers& reg, ALU& alu) {
    cout << "  [WARN] execute() not implemented for: " << name << endl;
}
