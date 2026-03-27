#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <string>
#include <map>
#include "Token.hpp"
#include <vector>

class Scanner
{
    private:
        std::map<std::string, TokenType> reservedWords;
    public:
        Scanner();
        std::vector<Token> scan(std::string sourceCode);
};

#endif