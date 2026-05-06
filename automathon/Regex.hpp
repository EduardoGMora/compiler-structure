#ifndef REGEX_HPP
#define REGEX_HPP

#include "Pattern.hpp"

class Regex {   // Regular Expression
    protected:
        Pattern* pattern;
    public:
        Regex(Pattern* p);
        virtual ~Regex();

        Pattern* getPattern() const;
        virtual bool isValid(const std::string& input) const = 0;
        virtual bool isAcceptedSymbol(char symbol) const = 0;
};

#endif // REGEX_HPP