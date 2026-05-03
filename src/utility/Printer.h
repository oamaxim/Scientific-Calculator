#pragma once
#include "../types/Value.h"
#include "../types/Matrix.h"

class Printer
{
public:
    static void printValue(const Value &v);
    static void printMatrix(const Matrix &m);
    static std::string formatDouble(double v);
};