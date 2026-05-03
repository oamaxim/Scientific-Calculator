#pragma once
#include "../types/Value.h"
#include "../types/Matrix.h"
#include "Utility.h"

class Printer
{
public:
    static void printValue(const Value &v);
    static void printMatrix(const Matrix &m);
    static void printCalcError(const std::string& input, const CalcError& e);
    static void printRuntimeError(const std::runtime_error& e);
    static std::string formatDouble(double v);
};