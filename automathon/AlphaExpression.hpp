#ifndef ALPHA_EXPRESSION_HPP
#define ALPHA_EXPRESSION_HPP

#include "Regex.hpp"

class AlphaExpression : public Regex {
    public:
        AlphaExpression(Pattern* p);
        virtual ~AlphaExpression();

        virtual bool isValid(const std::string& input) const override;
        virtual bool isAcceptedSymbol(char symbol) const override;
};


#endif // ALPHA_EXPRESSION_HPP