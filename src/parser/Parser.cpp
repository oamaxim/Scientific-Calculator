#include "Parser.h"
#include "../utility/Utility.h"
#include "../utility/CalcError.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens), pos(0) {}

Token Parser::peek()
{
    return tokens.at(pos);
}

Token &Parser::prev()
{
    return tokens[pos - 1];
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
    auto left = power();

    while (true)
    {
        TokenType t = peek().type;

        if (t == TokenType::STAR || t == TokenType::SLASH)
        {
            get();
            auto right = power();

            left = std::make_unique<BinaryNode>(
                (t == TokenType::STAR ? '*' : '/'),
                std::move(left),
                std::move(right));
        }
        else if (isImplicitMultiplication(prev().type, peek().type))
        {
            auto right = power();

            left = std::make_unique<BinaryNode>(
                '*',
                std::move(left),
                std::move(right));
        }
        else
            break;
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::power()
{
    auto left = unary();

    if (peek().type == TokenType::CARET)
    {
        get();
        auto right = power();

        return std::make_unique<BinaryNode>(
            '^',
            std::move(left),
            std::move(right));
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::unary()
{
    if (peek().type == TokenType::MINUS)
    {
        get();

        return std::make_unique<BinaryNode>(
            '-',
            std::make_unique<NumberNode>(0),
            unary());
    }
    
    if (peek().type == TokenType::PLUS){
        get();
        unary();
    }

    return factor();
}

std::unique_ptr<ASTNode> Parser::factor()
{
    Token t = get();
    if (t.type == TokenType::NUMBER)
        return std::make_unique<NumberNode>(t.value);

    if (t.type == TokenType::IDENT)
    {

        // Function call
        if (peek().type == TokenType::LPAREN)
        {
            get();

            std::vector<std::unique_ptr<ASTNode>> args;
            if (peek().type != TokenType::RPAREN)
            {
                args.push_back(expression());
                while (peek().type == TokenType::COMMA)
                {
                    get();
                    args.push_back(expression());
                }
            }
            get();
            return std::make_unique<FunctionNode>(t.name, std::move(args));
        }

        // Variable
        return std::make_unique<VariableNode>(t.name);
    }

    if (t.type == TokenType::LPAREN)
    {
        auto node = expression();
        get();
        return node;
    }

    throw CalcError(
        "Unexpected token '" + tokenToString(t) + "'",
        t.pos);
}

std::unique_ptr<ASTNode> Parser::parse()
{
    if (peek().type == TokenType::IDENT &&
        tokens[pos + 1].type == TokenType::ASSIGN)
    {
        std::string name = get().name;
        get();

        auto value = expression();
        return std::make_unique<AssignNode>(name, std::move(value));
    }
    return expression();
}

bool Parser::isImplicitMultiplication(TokenType left, TokenType right)
{
    bool leftValid =
        left == TokenType::NUMBER ||
        left == TokenType::IDENT ||
        left == TokenType::RPAREN;

    bool rightValid =
        right == TokenType::NUMBER ||
        right == TokenType::IDENT ||
        right == TokenType::LPAREN;

    return leftValid && rightValid;
}