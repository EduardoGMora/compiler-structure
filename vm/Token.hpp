#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

enum TokenType {
    // keywords
    TOKEN_START, TOKEN_END, TOKEN_IF, TOKEN_ELSE, TOKEN_WHILE, TOKEN_FOR, TOKEN_RETURN,
    
    // operators
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULTIPLY, TOKEN_DIVIDE, TOKEN_ASSIGN,
    
    // punctuation
    TOKEN_SEMICOLON, TOKEN_COMMA, TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_LBRACE, TOKEN_RBRACE, TOKEN_LBRACKET, TOKEN_RBRACKET,
    
    // literals
    TOKEN_INTEGER, TOKEN_FLOAT, TOKEN_STRING, TOKEN_CHAR, TOKEN_BOOL, TOKEN_VOID,
    
    // identifiers
    TOKEN_IDENTIFIER,
};

class Token
{
    private:
        TokenType type;
        std::string value;
    public:
        Token(TokenType type, std::string value);
        ~Token();
        TokenType getType() const;
        std::string getValue() const;

        static std::string typeName(TokenType type);
};

#endif