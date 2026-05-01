#pragma once
#include "../types/Token.h"
#include <string>
#include <vector>


class Lexer
{
private:
    std::string input;
    size_t pos;

public:
    Lexer(const std::string &input);
    std::vector<Token> tokenise();
};