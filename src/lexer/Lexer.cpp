#include "Lexer.h"
#include <cctype>
#include <stdexcept>
#include <iostream>

Lexer::Lexer(const std::string &input) : input(input), pos(0) {}

std::vector<Token> Lexer::tokenise()
{
    std::vector<Token> tokens;

    while (pos < input.size())
    {
        char current = input[pos];

        if (isspace(current))
        {
            pos++;
            continue;
        }

        if (isdigit(current))
        {
            std::string num;

            while (pos < input.size() && isdigit(input[pos]))
            {
                num += input[pos];
                pos++;
            }

            tokens.push_back({TokenType::NUMBER,
                              std::stod(num)});

            continue;
        };

        if (isalpha(current)){
            std::string name;

            while (pos<input.size() && isalnum(input[pos])){
                name += input[pos];
                pos++;
            }

            tokens.push_back({TokenType::INDENT});
            tokens.back().value = 0;
            tokens.back().name = name;
            continue;

        }

        switch (current)
        {
        case '+':
            tokens.push_back({TokenType::PLUS});
            break;

        case '-':
            tokens.push_back({TokenType::MINUS});
            break;

        case '*':
            tokens.push_back({TokenType::STAR});
            break;

        case '/':
            tokens.push_back({TokenType::SLASH});
            break;

        case '(':
            tokens.push_back({TokenType::LPAREN});
            break;

        case ')':
            tokens.push_back({TokenType::RPAREN});
            break;

        case '=':
            tokens.push_back({TokenType::ASSIGN});
            break;

        case '^':
            tokens.push_back({TokenType::CARET});
            break;

        default:
            throw std::runtime_error("Invalid character");
            break;
        }
        pos++;
    }

    tokens.push_back({TokenType::END});
    return tokens;
}