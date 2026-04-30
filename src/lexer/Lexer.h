#pragma once
#include <string>
#include <vector>
#include "../types/Token.h"

class Lexer
{
private:
    std::string input;
    size_t pos;

public:
    Lexer(const std::string &input);
    std::vector<Token> tokenise();
};