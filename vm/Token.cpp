#include "Token.hpp"

using namespace std;

Token::Token(TokenType type, string value) : type(type), value(value) {}

Token::~Token() {}

TokenType Token::getType() const { return type; }

string Token::getValue() const { return value; }

string Token::typeName(TokenType type)
{
    return to_string(type);
}