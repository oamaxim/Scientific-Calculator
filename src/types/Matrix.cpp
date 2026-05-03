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

Matrix Matrix::add(const Matrix &A, const Matrix &B)
{
    if (A.rows != B.rows || A.cols != B.cols)
        throw std::runtime_error("Matrix size mismatch");

    Matrix R(A.rows, A.cols);

    for (int i = 0; i < A.rows * A.cols; i++)
        R.data[i] = A.data[i] + B.data[i];

    return R;
}

Matrix Matrix::subtract(const Matrix &A, const Matrix &B)
{
    if (A.rows != B.rows || A.cols != B.cols)
        throw std::runtime_error("Matrix size mismatch");

    Matrix R(A.rows, A.cols);

    for (int i = 0; i < A.rows * A.cols; i++)
        R.data[i] = A.data[i] - B.data[i];

    return R;
}

Matrix Matrix::multiply(const Matrix &A, const Matrix &B)
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

Matrix Matrix::scalarMultiply(const Matrix &M, double s)
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

double Matrix::determinant(const Matrix &M)
{
    if (M.rows != M.cols)
        throw std::runtime_error("Matrix must be square");

    int n = M.rows;
    std::vector<double> a = M.data;

    double det = 1;

    for (int col = 0; col < n; col++)
    {
        int pivot = -1;

        for (int row = col; row < n; row++)
        {
            if (std::abs(a[row * n + col]) > 1e-12)
            {
                pivot = row;
                break;
            }
        }

        if (pivot < 0)
            return 0.0;

        if (pivot != col)
        {
            for (int k = 0; k < n; k++)
                std::swap(a[pivot * n + k], a[col * n + k]);

            det = -det;
        }

        det *= a[col * n + col];

        double piv = a[col * n + col];

        for (int row = col + 1; row < n; row++)
        {
            double f = a[row * n + col] / piv;

            for (int k = col; k < n; k++)
                a[row * n + k] -= f * a[col * n + k];
        }
    }

    return det;
}

Matrix Matrix::transpose(const Matrix &M)
{
    Matrix R(M.cols, M.rows);

    for (int i = 0; i < M.rows; i++)
        for (int j = 0; j < M.cols; j++)
            R.at(j, i) = M.at(i, j);

    return R;
}

Matrix Matrix::inverse(const Matrix &M)
{
    if (M.rows != M.cols)
        throw std::runtime_error("Matrix must be square");

    int n = M.rows;

    Matrix A = M;
    Matrix I = identity(n);

    for (int i = 0; i < n; i++)
    {
        double pivot = A.at(i, i);

        if (std::abs(pivot) < 1e-12)
            throw std::runtime_error("Matrix is singular (no inverse)");

        // Normalize row
        for (int j = 0; j < n; j++)
        {
            A.at(i, j) /= pivot;
            I.at(i, j) /= pivot;
        }

        // Eliminate other rows
        for (int k = 0; k < n; k++)
        {
            if (k == i)
                continue;

            double factor = A.at(k, i);

            for (int j = 0; j < n; j++)
            {
                A.at(k, j) -= factor * A.at(i, j);
                I.at(k, j) -= factor * I.at(i, j);
            }
        }
    }

    return I;
}