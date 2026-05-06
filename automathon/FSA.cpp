#include "FSA.hpp"
#include <cctype>

FSA::FSA(Regex* expr) {
    initialState = 0;
    acceptState = 1;
    currentState = initialState;
    regex = expr;
}

FSA::~FSA() {
    delete regex;
    regex = nullptr;
}

void FSA::reset() {
    currentState = initialState;
}

bool FSA::transition(char symbol) {
    switch (currentState) {
        case 0:
            if (std::isalpha(static_cast<unsigned char>(symbol))) {
                currentState = 1;
                return true;
            }
            return false;

        case 1:
            if (std::isalpha(static_cast<unsigned char>(symbol))) {
                currentState = 1;
                return true;
            }
            return false;

        default:
            return false;
    }
}

bool FSA::process(const std::string& input) {
    reset();

    if (input.empty()) {
        return false;
    }

    for (size_t i = 0; i < input.length(); i++) {
        if (!transition(input[i])) {
            return false;
        }
    }

    return isAcceptingState() && regex->isValid(input);
}

bool FSA::isAcceptingState() const {
    return currentState == acceptState;
}

int FSA::getCurrentState() const {
    return currentState;
}