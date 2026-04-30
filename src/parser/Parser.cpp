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

bool Parser::match(TokenType type)
{
    if (peek().type == type)
    {
        get();
        return true;
    }
    return false;
}

double Parser::expression()
{
    double result = term();
    while (peek().type == TokenType::PLUS || peek().type == TokenType::MINUS)
    {
        Token op = get();
        double right = term();

        if (op.type == TokenType::PLUS)
            result += right;
        else
            result -= right;
    }
    return result;
}

double Parser::term()
{
    double result = factor();
    while (peek().type == TokenType::STAR || peek().type == TokenType::SLASH)
    {
        Token op = get();
        double right = term();

        if (op.type == TokenType::STAR)
            result *= right;
        else
            result /= right;
    }
    return result;
}

double Parser::factor()
{
    Token t = get();
    if (t.type == TokenType::NUMBER)
        return t.value;

    if (t.type == TokenType::LPAREN)
    {
        double result = expression();
        get();
        return result;
    }

    throw std::runtime_error("Unexpected token");
}

double Parser::parse()
{
    return expression();
}