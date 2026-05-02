#pragma once
#include <vector>
#include <stdexcept>

class Matrix
{
public:
    int rows, cols;
    std::vector<double> data;

    Matrix() : rows(0), cols(0) {}
    Matrix(int r, int c) : rows(r), cols(c), data(r * c, 0.0) {}

    double &at(int r, int c)
    {
        return data[r * cols + c];
    };

    double at(int r, int c) const
    {
        return data[r * cols + c];
    };
};
