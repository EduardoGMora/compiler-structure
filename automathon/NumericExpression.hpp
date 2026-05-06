#ifndef NUMERIC_EXPRESSION_HPP
#define NUMERIC_EXPRESSION_HPP

#include "Regex.hpp"

class NumericExpression : public Regex {
    public:
        NumericExpression(Pattern* p);
        virtual ~NumericExpression();

        virtual bool isValid(const std::string& input) const override;
        virtual bool isAcceptedSymbol(char symbol) const override;
};

#endif // NUMERIC_EXPRESSION_HPP
