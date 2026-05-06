#ifndef FSA_HPP
#define FSA_HPP

#include <string>
#include "Regex.hpp"

class FSA {     // Finite State Automaton
    private:
        int currentState;
        int initialState;
        int acceptState;

        Regex* regex;

    public:
        FSA(Regex* r);
        ~FSA();

        void reset();
        bool transition(char symbol);
        bool process(const std::string& input);
        bool isAcceptingState() const;

        int getCurrentState() const;
};

#endif // FSA_HPP