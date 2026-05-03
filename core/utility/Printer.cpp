#include "Printer.h"
#include "Utility.h"
#include <iostream>
#include <iomanip>
#include <variant>
#include <sstream>

void Printer::printValue(const Value &v)
{
    if (std::holds_alternative<double>(v))
    {
        std::cout << "= " << formatDouble(std::get<double>(v)) << "\n";
    }
    else if (std::holds_alternative<Matrix>(v))
    {
        printMatrix(std::get<Matrix>(v));
    }
}

void Printer::printMatrix(const Matrix &m)
{
    std::cout << "Matrix (" << m.rows << "x" << m.cols << "):\n";

    std::vector<std::vector<std::string>> str(m.rows, std::vector<std::string>(m.cols));

    for (int i = 0; i < m.rows; i++)
    {
        for (int j = 0; j < m.cols; j++)
        {
            
            str[i][j] = formatDouble(m.at(i, j));
        }
    }

    std::vector<size_t> colWidth(m.cols, 0);

    for (int j = 0; j < m.cols; j++)
    {
        for (int i = 0; i < m.rows; i++)
        {
            colWidth[j] = std::max(colWidth[j], str[i][j].length());
        }
    }

    for (int i = 0; i < m.rows; i++)
    {
        for (int j = 0; j < m.cols; j++)
        {
            std::cout << std::setw(colWidth[j]) << str[i][j] << " ";
        }
        std::cout << "\n";
    }
}
void Printer::printCalcError(const std::string &input, const CalcError &e)
{
    std::cout << input << std::endl;
    std::cout << std::string(e.pos, ' ') << "^" << std::endl;
    std::cout << "Error: " << e.what() << std::endl;
}

void Printer::printRuntimeError(const std::runtime_error &e)
{
    std::cout << "Error: " << e.what() << std::endl;
}

std::string Printer::formatDouble(double v)
{
    if (std::abs(v) < 1e-12)
    {
        v = 0;
    }

    std::ostringstream ss;
    ss << std::setprecision(10) << v;

    std::string s = ss.str();

    if (s.find('.') != std::string::npos)
    {
        s.erase(s.find_last_not_of('0') + 1);
        if (s.back() == '.')
            s.pop_back();
    }

    return s;
}