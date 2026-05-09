#pragma once
#include <variant>
#include "Matrix.h"

using Value = std::variant<double, Matrix>;

namespace ValueUtil
{
    double asNumber(const Value &v);
    Matrix asMatrix(const Value &v);
    Value add(const Value &a, const Value &b);
    Value subtract(const Value &a, const Value &b);
    Value multiply(const Value &a, const Value &b);
    Value divide(const Value &a, const Value &b);
    Value power(const Value &a, const Value &b);
    bool isMultiple(double value, double base, double epsilon);
};