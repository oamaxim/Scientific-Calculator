#include "Utility.h"

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