#include "Parser.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens), pos(0) {}

Token Parser::peek()
{
    return tokens.at(pos);
}

Token Parser::get()
{
    return tokens.at(pos++);
}

// bool Parser::match(TokenType type)
// {
//     if (peek().type == type)
//     {
//         get();
//         return true;
//     }
//     return false;
// }

std::unique_ptr<ASTNode> Parser::expression()
{
    auto left = term();

    while (peek().type == TokenType::PLUS ||
           peek().type == TokenType::MINUS)
    {
        char op = get().type == TokenType::PLUS ? '+' : '-';
        auto right = term();

        left = std::make_unique<BinaryNode>(
            op,
            std::move(left),
            std::move(right));
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::term()
{
    auto left = factor();

    while (peek().type == TokenType::STAR ||
           peek().type == TokenType::SLASH)
    {
        char op = get().type == TokenType::STAR ? '*' : '/';
        auto right = factor();

        left = std::make_unique<BinaryNode>(
            op,
            std::move(left),
            std::move(right));
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::factor()
{
    Token t = get();
    if (t.type == TokenType::NUMBER)
        return std::make_unique<NumberNode>(t.value);

    if (t.type == TokenType::LPAREN)
    {
        auto node = expression();
        get();
        return node;
    }

    throw std::runtime_error("Invalid Expression");
}

std::unique_ptr<ASTNode> Parser::parse()
{
    return expression();
}