#include "Printer.h"
#include "Utility.h"
#include <iostream>
#include <iomanip>
#include <variant>

void Printer::printValue(const Value &v)
{
    if (std::holds_alternative<double>(v))
    {
        std::cout << "= " << formatDouble(nearZero(std::get<double>(v))) << "\n";
    }
    else if (std::holds_alternative<Matrix>(v))
    {
        printMatrix(std::get<Matrix>(v));
    }
}

void Printer::printMatrix(const Matrix& m)
{
    std::cout << "Matrix (" << m.rows << "x" << m.cols << "):\n";

    for (int i = 0; i < m.rows; i++)
    {
        std::cout << "  ";
        for (int j = 0; j < m.cols; j++)
        {
            std::cout << std::setw(10) << formatDouble(nearZero(m.at(i, j))) << " ";
        }
        std::cout << "\n";
    }
}

std::string Printer::formatDouble(double v)
{
    std::ostringstream ss;
    ss << std::setprecision(10) << v;

    std::string s = ss.str();

    if (s.find('.') != std::string::npos)
    {
        s.erase(s.find_last_not_of('0') + 1);
        if (s.back() == '.') s.pop_back();
    }

    return s;
}