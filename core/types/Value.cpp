#include "Value.h"
#include <cmath>

double ValueUtil::asNumber(const Value &v)
{
    if (!std::holds_alternative<double>(v))
        throw std::runtime_error("Expected number");
    return std::get<double>(v);
}

Matrix ValueUtil::asMatrix(const Value &v)
{
    if (!std::holds_alternative<Matrix>(v))
        throw std::runtime_error("Expected matrix");
    return std::get<Matrix>(v);
}

Value ValueUtil::add(const Value &a, const Value &b)
{
    if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b))
        return std::get<double>(a) + std::get<double>(b);

    if (std::holds_alternative<Matrix>(a) && std::holds_alternative<Matrix>(b))
        return Matrix::add(std::get<Matrix>(a), std::get<Matrix>(b));

    throw std::runtime_error("Invalid addition types");
}

Value ValueUtil::subtract(const Value &a, const Value &b)
{
    if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b))
        return std::get<double>(a) - std::get<double>(b);

    if (std::holds_alternative<Matrix>(a) && std::holds_alternative<Matrix>(b))
        return Matrix::subtract(std::get<Matrix>(a), std::get<Matrix>(b));

    throw std::runtime_error("Invalid subtraction types");
}

Value ValueUtil::multiply(const Value &a, const Value &b)
{
    if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b))
        return std::get<double>(a) * std::get<double>(b);

    if (std::holds_alternative<Matrix>(a) && std::holds_alternative<Matrix>(b))
        return Matrix::multiply(std::get<Matrix>(a), std::get<Matrix>(b));

    if (std::holds_alternative<double>(a) && std::holds_alternative<Matrix>(b))
        return Matrix::scalarMultiply(std::get<double>(a), std::get<Matrix>(b));

    if (std::holds_alternative<Matrix>(a) && std::holds_alternative<double>(b))
        return Matrix::scalarMultiply(std::get<double>(b), std::get<Matrix>(a));

    throw std::runtime_error("Invalid multiplication types");
}
Value ValueUtil::divide(const Value &a, const Value &b)
{
    if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b))
    {
        if (std::get<double>(b) == 0)
        {
            throw std::runtime_error("Division by zero");
        }
        return std::get<double>(a) / std::get<double>(b);
    }

    if (std::holds_alternative<Matrix>(a) && std::holds_alternative<Matrix>(b))
        return Matrix::divide(std::get<Matrix>(a), std::get<Matrix>(b));

    throw std::runtime_error("Invalid division types");
}

Value ValueUtil::power(const Value &a, const Value &b)
{
    if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b))
        return std::pow(std::get<double>(a), std::get<double>(b));

    if (std::holds_alternative<Matrix>(a) && std::holds_alternative<double>(b))
        return Matrix::power(std::get<Matrix>(a), std::get<double>(b));

    throw std::runtime_error("Invalid power types");
}

bool ValueUtil::isMultiple(double value, double base, double epsilon = 1e-9)
{
    double ratio = value / base;
    return std::abs(ratio - std::round(ratio)) < epsilon;
}