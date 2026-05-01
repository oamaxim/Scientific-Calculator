#include "Lexer.h"
#include "../utility/CalcError.h"
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

            tokens.push_back({TokenType::NUMBER, std::stod(num), "", (int)pos});

            continue;
        };

        if (isalpha(current))
        {
            std::string name;
            int start = (int)pos;

            while (pos < input.size() && isalnum(input[pos]))
            {
                name += input[pos];
                pos++;
            }

            tokens.push_back({TokenType::IDENT, 0, name, start});
            continue;
        }

        switch (current)
        {
        case '+':
            tokens.push_back({TokenType::PLUS, 0, "", (int)pos});
            break;

        case '-':
            tokens.push_back({TokenType::MINUS, 0, "", (int)pos});
            break;

        case '*':
            tokens.push_back({TokenType::STAR, 0, "", (int)pos});
            break;

        case '/':
            tokens.push_back({TokenType::SLASH, 0, "", (int)pos});
            break;

        case '(':
            tokens.push_back({TokenType::LPAREN, 0, "", (int)pos});
            break;

        case ')':
            tokens.push_back({TokenType::RPAREN, 0, "", (int)pos});
            break;

        case '=':
            tokens.push_back({TokenType::ASSIGN, 0, "", (int)pos});
            break;

        case '^':
            tokens.push_back({TokenType::CARET, 0, "", (int)pos});
            break;

        case ',':
            tokens.push_back({TokenType::COMMA, 0, "", (int)pos});
            break;

        default:
            throw CalcError(
                std::string("Invalid character '") + current + "'",
                pos);
        }

        pos++;
    }

    tokens.push_back({TokenType::END});
    return tokens;
}