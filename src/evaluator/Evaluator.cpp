#include "Evaluator.h"
#include "../parser/AST.h"
#include "../utility/CalcError.h"
#include <stdexcept>
#include <cmath>
#include <vector>

Evaluator::Evaluator()
{
    constants["pi"] = M_PI;
    constants["e"] = M_E;
}

Value Evaluator::evaluate(ASTNode *node)
{
    // NUMBER
    if (auto num = dynamic_cast<NumberNode *>(node))
    {
        return num->value;
    }

    // VARIABLE
    if (auto var = dynamic_cast<VariableNode *>(node))
    {
        if (variables.count(var->name))
        {
            return variables[var->name];
        }

        if (constants.count(var->name))
        {
            return constants[var->name];
        }
        throw std::runtime_error("Undefined variable '" + var->name + "'");
    }

    // ASSIGNMENT
    if (auto assign = dynamic_cast<AssignNode *>(node))
    {
        if (constants.count(assign->name))
        {
            throw std::runtime_error("Cannot assign to constant '" + assign->name + "'");
        }

        Value val = evaluate(assign->value.get());

        variables[assign->name] = val;

        return val;
    }

    // BINARY
    if (auto bin = dynamic_cast<BinaryNode *>(node))
    {
        Value leftV = evaluate(bin->left.get());
        Value rightV = evaluate(bin->right.get());

        // Both Numbers
        if (std::holds_alternative<double>(leftV) &&
            std::holds_alternative<double>(rightV))
        {

            double left = std::get<double>(leftV);
            double right = std::get<double>(rightV);

            switch (bin->op)
            {
            case '+':
                return left + right;
            case '-':
                return left - right;
            case '*':
                return left * right;
            case '/':
                if (right == 0)
                    throw CalcError("Division by zero", 0);
                return left / right;
            case '^':
                return std::pow(left, right);
            }
        }

        // Both Matrix
        if (std::holds_alternative<Matrix>(leftV) &&
            std::holds_alternative<Matrix>(rightV))
        {

            const Matrix &A = std::get<Matrix>(leftV);
            const Matrix &B = std::get<Matrix>(rightV);

            if (bin->op == '+')
            {
                if (A.rows != B.rows || A.cols != B.cols)
                {
                    throw CalcError("Matrix size mismatch", 0);
                }
                Matrix R(A.rows, A.cols);
                for (int i = 0; i < A.rows * A.cols; i++)
                {
                    R.data[i] = A.data[i] + B.data[i];
                }
                return R;
            }

            if (bin->op == '-')
            {
                if (A.rows != B.rows || A.cols != B.cols)
                {
                    throw CalcError("Matrix size mismatch", 0);
                }
                Matrix R(A.rows, A.cols);
                for (int i = 0; i < A.rows * A.cols; i++)
                {
                    R.data[i] = A.data[i] - B.data[i];
                }
                return R;
            }

            if (bin->op == '*')
            {
                if (A.cols != B.rows)
                {
                    throw CalcError("Invalid matrix multiplication dimension", 0);
                }

                Matrix R(A.rows, B.cols);

                for (int i = 0; i < A.rows; i++)
                {
                    for (int j = 0; j < B.cols; j++)
                    {
                        double sum = 0;
                        for (int k = 0; k < A.cols; k++)
                        {
                            sum += A.at(i, k) * B.at(k, j);
                        }
                        R.at(i, j) = sum;
                    }
                }

                return R;
            }
        }

        // Number and Matrix
        if (std::holds_alternative<double>(leftV) &&
            std::holds_alternative<Matrix>(rightV) &&
            bin->op == '*')
        {
            double s = std::get<double>(leftV);
            const Matrix &M = std::get<Matrix>(rightV);

            Matrix R(M.rows, M.cols);
            for (int i = 0; i < M.rows * M.cols; i++)
                R.data[i] = s * M.data[i];

            return R;
        }

        // Matrix and Number
        if (std::holds_alternative<Matrix>(leftV) &&
            std::holds_alternative<double>(rightV) &&
            bin->op == '*')
        {
            const Matrix &M = std::get<Matrix>(leftV);
            double s = std::get<double>(rightV);

            Matrix R(M.rows, M.cols);
            for (int i = 0; i < M.rows * M.cols; i++)
                R.data[i] = s * M.data[i];

            return R;
        }

        if (std::holds_alternative<Matrix>(leftV) &&
            std::holds_alternative<double>(rightV) &&
            bin->op == '^')
        {
            const Matrix &A = std::get<Matrix>(leftV);
            int power = (int)std::get<double>(rightV);

            if (power < 0)
                throw CalcError("Matrix power must be non-negative", 0);

            if (A.rows != A.cols)
                throw CalcError("Matrix power requires square matrix", 0);

            // Identity matrix
            Matrix result(A.rows, A.cols);
            for (int i = 0; i < A.rows; i++)
                result.at(i, i) = 1;

            if (power == 0)
                return result;

            Matrix base = A;

            for (int p = 0; p < power; p++)
            {
                Matrix temp(A.rows, A.cols);

                for (int i = 0; i < A.rows; i++)
                {
                    for (int j = 0; j < A.cols; j++)
                    {
                        double sum = 0;
                        for (int k = 0; k < A.cols; k++)
                        {
                            sum += result.at(i, k) * base.at(k, j);
                        }
                        temp.at(i, j) = sum;
                    }
                }

                result = temp;
            }

            return result;
        }
    }

    if (auto func = dynamic_cast<FunctionNode *>(node))
    {
        std::vector<double> values;
        for (auto &arg : func->args)
        {
            values.push_back(asNumber(evaluate(arg.get())));
        }
        // Trig
        if (func->name == "sin")
            return std::sin(values[0]);
        if (func->name == "cos")
            return std::cos(values[0]);
        if (func->name == "tan")
            return std::tan(values[0]);

        // Reciprocal Trig
        if (func->name == "cosec")
            return 1 / std::sin(values[0]);
        if (func->name == "sec")
            return 1 / std::cos(values[0]);
        if (func->name == "cot")
            return 1 / std::tan(values[0]);

        // Inverse Trig
        if (func->name == "arcsin")
            return std::asin(values[0]);
        if (func->name == "arccos")
            return std::acos(values[0]);
        if (func->name == "arctan")
            return std::atan(values[0]);

        // Inverse Reciprocal Trig
        if (func->name == "arccsc")
            return std::asin(1 / values[0]);
        if (func->name == "arcsec")
            return std::acos(1 / values[0]);
        if (func->name == "arccot")
            return std::atan(1 / values[0]);

        // Hyperbolic
        if (func->name == "sinh")
            return std::sinh(values[0]);
        if (func->name == "cosh")
            return std::cosh(values[0]);
        if (func->name == "tanh")
            return std::tanh(values[0]);

        // Inverse Hyperbolic
        if (func->name == "arcsinh")
            return std::asinh(values[0]);
        if (func->name == "arccosh")
            return std::acosh(values[0]);
        if (func->name == "arctanh")
            return std::atanh(values[0]);

        // Matricies
        if (func->name == "mat")
        {
            auto &args = func->args;
            if (args.size() < 2)
            {
                throw CalcError("Matrix requires rows and columns", 0);
            }

            int r = (int)asNumber(evaluate(args[0].get()));
            int c = (int)asNumber(evaluate(args[1].get()));

            Matrix m(r, c);

            int expected = r * c;

            if ((int)func->args.size() - 2 != expected)
            {
                throw CalcError("Incorrect number of matrix values", 0);
            }

            for (int i = 0; i < expected; i++)
            {
                m.data[i] = asNumber(evaluate(args[i + 2].get()));
            }

            return m;
        }

        if (func->name == "det")
        {
            if (func->args.size() != 1)
            {
                throw CalcError("det() expects 1 matrix argument", 0);
            }

            Matrix m = asMatrix(evaluate(func->args[0].get()));

            if (m.rows != m.cols)
            {
                throw CalcError("Matrix must be square", 0);
            }

            int n = m.rows;
            std::vector<double> a = m.data;

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
                {
                    return 0.0;
                }
                if (pivot != col)
                {
                    for (int k = 0; k < n; k++)
                    {
                        std::swap(a[pivot * n + k], a[col * n + k]);
                    }

                    det = -det;
                }

                det *= a[col * n + col];

                double piv = a[col * n + col];

                for (int row = col + 1; row < n; row++)
                {
                    double f = a[row * n + col] / piv;

                    for (int k = col; k < n; k++)
                    {
                        a[row * n + k] -= f * a[col * n + k];
                    }
                }
            }

            return det;
        }

        // Others
        if (func->name == "sqrt")
            return std::sqrt(values[0]);

        if (func->name == "abs")
            return std::abs(values[0]);

        throw std::runtime_error("Unknown function '" + func->name + "'");
    }

    throw std::runtime_error("Unknown AST node");
}

double Evaluator::asNumber(const Value &v)
{
    if (!std::holds_alternative<double>(v))
    {
        throw CalcError("Expected number", 0);
    }
    return std::get<double>(v);
}

const Matrix &Evaluator::asMatrix(const Value &v)
{
    if (!std::holds_alternative<Matrix>(v))
    {
        throw CalcError("Expected matrix", 0);
    }
    return std::get<Matrix>(v);
}