#pragma once
#include "types/Value.h"
#include "types/Matrix.h"
#include "Utility.h"

class Printer
{
public:
    static void printValue(const Value &v);
    static void printMatrix(const Matrix &m);
    static std::string formatCalcError(const std::string &input, const CalcError &e);
    static std::string formatRuntimeError(const std::exception &e);
    static std::string formatDouble(double v);
    static std::string toString(const Value &v);

private:
    static std::string matrixToString(const Matrix &m);
};