#include "Matrix.h"

Matrix::Matrix(int r, int c) : rows(r), cols(c), data(r * c, 0.0) {}

double &Matrix::at(int r, int c)
{
    return data[r * cols + c];
}

double Matrix::at(int r, int c) const
{
    return data[r * cols + c];
}

Matrix Matrix::add(const Matrix& A, const Matrix& B)
{
    if (A.rows != B.rows || A.cols != B.cols)
        throw std::runtime_error("Matrix size mismatch");

    Matrix R(A.rows, A.cols);

    for (int i = 0; i < A.rows * A.cols; i++)
        R.data[i] = A.data[i] + B.data[i];

    return R;
}

Matrix Matrix::subtract(const Matrix& A, const Matrix& B)
{
    if (A.rows != B.rows || A.cols != B.cols)
        throw std::runtime_error("Matrix size mismatch");

    Matrix R(A.rows, A.cols);

    for (int i = 0; i < A.rows * A.cols; i++)
        R.data[i] = A.data[i] - B.data[i];

    return R;
}

Matrix Matrix::multiply(const Matrix& A, const Matrix& B)
{
    if (A.cols != B.rows)
        throw std::runtime_error("Invalid matrix multiplication");

    Matrix R(A.rows, B.cols);

    for (int i = 0; i < A.rows; i++)
    {
        for (int j = 0; j < B.cols; j++)
        {
            double sum = 0;

            for (int k = 0; k < A.cols; k++)
                sum += A.at(i, k) * B.at(k, j);

            R.at(i, j) = sum;
        }
    }

    return R;
}

Matrix Matrix::scalarMultiply(const Matrix& M, double s)
{
    Matrix R(M.rows, M.cols);

    for (int i = 0; i < M.rows * M.cols; i++)
        R.data[i] = M.data[i] * s;

    return R;
}

Matrix Matrix::identity(int n)
{
    Matrix I(n, n);

    for (int i = 0; i < n; i++)
        I.at(i, i) = 1;

    return I;
}

Matrix Matrix::power(Matrix base, int exp)
{
    if (base.rows != base.cols)
        throw std::runtime_error("Matrix must be square");

    Matrix result = identity(base.rows);

    while (exp > 0)
    {
        if (exp % 2 == 1)
            result = multiply(result, base);

        base = multiply(base, base);
        exp /= 2;
    }

    return result;
}