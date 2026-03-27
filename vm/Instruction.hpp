#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <string>

// Forward declarations to avoid circular includes
class Registers;
class ALU;

using namespace std;

class Instruction 
{
	protected: 
		string name;
		short int code;
		short int length;
	public:
		Instruction();
		Instruction(string theName, short int theCode, short int theLength);
		void display();
		int getCode();
		string getName();
		virtual void execute(Registers& reg, ALU& alu);
		virtual ~Instruction() = default;
};
#endif