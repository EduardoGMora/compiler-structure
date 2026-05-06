#include <iostream>
#include "Pattern.hpp"
#include "FSA.hpp"
#include "AlphaExpression.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    Pattern* p = new Pattern("[A-Za-z]+");
    cout << "Pattern: " << p->getPattern() << endl;

    Regex* alphaExp = new AlphaExpression(p);

    FSA* fsa = new FSA(alphaExp);

    string input;

    if (argc > 1) {
        input = argv[1];
    } else {
        cout << "Enter a string to validate: ";
        cin >> input;
    }   

    cout << "Input: " << input << endl;
    
    if(fsa->process(input))
        cout << "ACCEPTED\n";
    else
        cout << "REJECTED\n";

    delete fsa;

    return 0;
}