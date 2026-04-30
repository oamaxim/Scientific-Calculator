#pragma once
#include <string>

enum class TokenType {
    NUMBER,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    LPAREN,
    RPAREN,
    END
};

struct Token {
    TokenType type;
    double value = 0;
};