#include "AlphaExpression.hpp"
#include <cctype>

AlphaExpression::AlphaExpression(Pattern* p)
    : Regex(p) {
}

AlphaExpression::~AlphaExpression() {
}

bool AlphaExpression::isValid(const std::string& input) const {

    if (input.empty())
        return false;

    for(char c : input)
        if(!std::isalpha(static_cast<unsigned char>(c)))
            return false;

    return true;
}