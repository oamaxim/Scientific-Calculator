#pragma once
#include <string>

enum class TokenType
{
    NUMBER,
    INDENT,
    PLUS, MINUS, STAR, SLASH, CARET,
    LPAREN, RPAREN, COMMA,
    ASSIGN,
    END
};

struct Token
{
    TokenType type;
    double value = 0;
    std::string name;
};