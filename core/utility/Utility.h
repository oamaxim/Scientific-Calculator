#pragma once
#include "types/Token.h"
#include "types/Matrix.h"
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
std::string trim(const std::string& s);
double logbase(double a, double base);