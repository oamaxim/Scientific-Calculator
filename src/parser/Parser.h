#pragma once
#include <vector>
#include "../types/Token.h"

class Parser
{
    private:
    std::vector<Token> tokens;
    size_t pos;

    public:
    Parser(const std::vector<Token>& tokens);
    double parse();

    private:
    double expression();
    double term();
    double factor();

    Token peek();
    Token get();
    bool match(TokenType type);
};