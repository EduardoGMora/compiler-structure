#include <iostream>
#include <string>
#include "Pattern.hpp"
#include "FSA.hpp"
#include "AlphaExpression.hpp"
#include "NumericExpression.hpp"

int main() {
    int option = 0;
    std::string input;

    do {
        std::cout << "==========================================\n";
        std::cout << " Deterministic Finite State Automata (FSA)\n";
        std::cout << " 1. Validate alphabetic string [A-Za-z]+\n";
        std::cout << " 2. Validate numeric string [0-9]+\n";
        std::cout << " 3. Exit\n";
        std::cout << "==========================================\n";
        std::cout << "Choose an option: ";
        std::cin >> option;
        std::cin.ignore();

        if (option == 1) {
            Pattern* pattern = new Pattern("[A-Za-z]+");
            Regex* expression = new AlphaExpression(pattern);
            FSA* fsa = new FSA(expression);

            std::cout << "Enter an alphabetic string: ";
            std::getline(std::cin, input);

            if (fsa->process(input))
                std::cout << "Result: ACCEPTED\n";
            else
                std::cout << "Result: REJECTED\n";

            std::cout << "Final state: q" << fsa->getCurrentState() << "\n\n";
            delete fsa;
        }
        else if (option == 2) {
            Pattern* pattern = new Pattern("[0-9]+");
            Regex* expression = new NumericExpression(pattern);
            FSA* fsa = new FSA(expression);

            std::cout << "Enter a numeric string: ";
            std::getline(std::cin, input);

            if (fsa->process(input))
                std::cout << "Result: ACCEPTED\n";
            else
                std::cout << "Result: REJECTED\n";

            std::cout << "Final state: q" << fsa->getCurrentState() << "\n\n";
            delete fsa;
        }
        else if (option != 3) {
            std::cout << "Invalid option.\n\n";
        }

    } while (option != 3);

    std::cout << "Program finished.\n";
    return 0;
}
