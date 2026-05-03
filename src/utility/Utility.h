#pragma once
#include "../types/Token.h"
#include "../types/Matrix.h"
#include <stdexcept>
#include <string>

class CalcError : public std::runtime_error
{
public:
    int pos;

    CalcError(const std::string &message, int position)
        : std::runtime_error(message), pos(position) {}
};

std::string tokenToString(const Token &t);
void printMatrix(const Matrix& M);