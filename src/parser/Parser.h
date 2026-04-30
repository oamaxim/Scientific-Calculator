#pragma once
#include <vector>
#include <memory>
#include "../types/Token.h"
#include "AST.h"

class Parser
{
private:
    std::vector<Token> tokens;
    size_t pos;

public:
    Parser(const std::vector<Token> &tokens);
    std::unique_ptr<ASTNode> parse();

private:
    std::unique_ptr<ASTNode> expression();
    std::unique_ptr<ASTNode> term();
    std::unique_ptr<ASTNode> factor();

    Token peek();
    Token get();
    // bool match(TokenType type);
};