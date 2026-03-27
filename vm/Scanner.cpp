#include <iostream>
#include "Scanner.hpp"

using namespace std;

Scanner::Scanner()
{
    reservedWords = {
        {"if",     TokenType::TOKEN_IF},
        {"else",   TokenType::TOKEN_ELSE},
        {"while",  TokenType::TOKEN_WHILE},
        {"for",    TokenType::TOKEN_FOR},
        {"return", TokenType::TOKEN_RETURN},
        {"int",    TokenType::TOKEN_INTEGER},
        {"float",  TokenType::TOKEN_FLOAT},
        {"string", TokenType::TOKEN_STRING},
        {"void",   TokenType::TOKEN_VOID},
        {"char",   TokenType::TOKEN_CHAR},
        {"bool",   TokenType::TOKEN_BOOL},

        // arithmetic operators
        {"+", TokenType::TOKEN_PLUS},
        {"-", TokenType::TOKEN_MINUS},
        {"*", TokenType::TOKEN_MULTIPLY},
        {"/", TokenType::TOKEN_DIVIDE},
        {"=", TokenType::TOKEN_ASSIGN},

        // punctuation
        {";", TokenType::TOKEN_SEMICOLON},
        {",", TokenType::TOKEN_COMMA},
        {"(", TokenType::TOKEN_LPAREN},
        {")", TokenType::TOKEN_RPAREN},
        {"{", TokenType::TOKEN_LBRACE},
        {"}", TokenType::TOKEN_RBRACE},
        {"[", TokenType::TOKEN_LBRACKET},
        {"]", TokenType::TOKEN_RBRACKET},
    };
}

std::vector<Token> Scanner::scan(std::string sourceCode)
{
    std::vector<Token> tokens;

    std::cout << "Scanning: " << sourceCode << std::endl << std::endl;

    int position = 0;
    while (position < (int)sourceCode.size()) {
        // Skip whitespace
        while (position < (int)sourceCode.size() && isspace(sourceCode[position])) {
            position++;
        }

        if (position >= (int)sourceCode.size()) break;

        // Form a word from letters/digits
        if (isalpha(sourceCode[position]) || sourceCode[position] == '_') {
            size_t start = position;
            while (position < (int)sourceCode.size() && (isalnum(sourceCode[position]) || sourceCode[position] == '_')) {
                position++;
            }
            std::string word = sourceCode.substr(start, position - start);

            if (reservedWords.find(word) != reservedWords.end()) {
                TokenType type = reservedWords[word];
                std::string tokenName = Token::typeName(type);
                std::cout << "  " << word << " -> " << tokenName << std::endl;
                tokens.push_back(Token{type, word});
            } else {
                std::cout << "  " << word << " -> TOKEN_IDENTIFIER" << std::endl;
                tokens.push_back(Token{TokenType::TOKEN_IDENTIFIER, word});
            }
        } else {
            // Skip non-alpha characters (operators, punctuation, numbers for now)
            position++;
        }
    }

    return tokens;
}