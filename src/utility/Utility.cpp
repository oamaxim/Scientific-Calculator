#include "Utility.h"
#include <iostream>
#include <iomanip>
std::string tokenToString(const Token &t)
{
    switch (t.type)
    {
    case TokenType::PLUS:
        return "+";
    case TokenType::MINUS:
        return "-";
    case TokenType::STAR:
        return "*";
    case TokenType::SLASH:
        return "/";
    case TokenType::CARET:
        return "^";
    case TokenType::ASSIGN:
        return "=";
    case TokenType::LPAREN:
        return "(";
    case TokenType::RPAREN:
        return ")";
    case TokenType::COMMA:
        return ",";
    case TokenType::IDENT:
        return t.name.empty() ? "<identifier>" : t.name;
    case TokenType::NUMBER:
        return std::to_string(t.value);
    default:
        return "<unknown>";
    }
}

std::string trim(const std::string &s)
{
    size_t start = s.find_first_not_of(" \t\n\r");
    size_t end = s.find_last_not_of(" \t\n\r");

    if (start == std::string::npos)
        return "";
    return s.substr(start, end - start + 1);
}
