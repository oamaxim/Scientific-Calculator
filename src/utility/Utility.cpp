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

void printMatrix(const Matrix &M)
{
    std::cout << "Matrix (" << M.rows << "x" << M.cols << "):\n";

    for (int i = 0; i < M.rows; i++)
    {
        for (int j = 0; j < M.cols; j++)
        {
            std::cout << std::setw(8) << M.at(i, j) << " ";
        }
        std::cout << "\n";
    }
}

