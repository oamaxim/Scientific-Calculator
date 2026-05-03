#pragma once
#include <vector>
#include <stdexcept>

class Matrix
{
public:
    int rows, cols;
    std::vector<double> data;

    Matrix(int r, int c);

    double &at(int r, int c);
    double at(int r, int c) const;

    static Matrix add(const Matrix &A, const Matrix &B);
    static Matrix subtract(const Matrix &A, const Matrix &B);
    static Matrix multiply(const Matrix &A, const Matrix &B);
    static Matrix scalarMultiply(const Matrix &M, double s);
    static Matrix identity(int n);
    static Matrix power(Matrix base, int exp);

    static double Matrix::determinant(const Matrix& M);
    static Matrix transpose(const Matrix& M);
    static Matrix inverse(const Matrix& M);
};