#pragma once
#include "../types/Token.h"
#include "AST.h"
#include <vector>
#include <memory>


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
    std::unique_ptr<ASTNode> power();
    std::unique_ptr<ASTNode> unary();
    std::unique_ptr<ASTNode> factor();
    bool isImplicitMultiplication(TokenType left, TokenType right);


    Token peek();
    Token get();
    Token& prev();
};